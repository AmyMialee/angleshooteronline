#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

int main(int, char*[]) {
	try {
		AngleShooterClient::get().run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterClient::AngleShooterClient() :
	foundAddress(127, 0, 0, 1),
	window(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Titlebar | sf::Style::Close),
	renderTexture({960, 540}),
	tps(static_cast<int>(1 / AngleShooterCommon::TIME_PER_TICK)),
	fps(144)
{
    NetworkProtocol::initialize();
	window.clear();
	window.setKeyRepeatEnabled(false);
	StateManager::get().push(SplashState::getId());
	OptionsManager::get().loadFromFile();
	connectingSocket.setBlocking(false);
	registerPacket(NetworkProtocol::S2C_INITIAL_SETUP, [this](sf::Packet& packet) {
		Logger::debug("Received Initial Setup Packet");
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
		packet >> this->playerId;
	});
	registerPacket(NetworkProtocol::S2C_BROADCAST_MESSAGE, [this](sf::Packet& packet) {
		std::string message;
		packet >> message;
		Logger::debug("Received Broadcast Message Packet from server: " + message);
	});
	registerPacket(NetworkProtocol::PACKET_QUESTION, [this](sf::Packet& packet) {
		uint8_t packetId;
		packet >> packetId;
		auto translation = NetworkProtocol::PACKET_TRANSLATION.getPacket();
		translation << packetId;
		translation << this->packetIds[packetId];
		Logger::debug("Received Translation Question Packet: " + this->packetIds[packetId].toString() + " for packet id: " + std::to_string(packetId));
		send(translation);
	});
	registerPacket(NetworkProtocol::PACKET_TRANSLATION, [this](sf::Packet& packet) {
		uint8_t packetId;
		Identifier translation;
		packet >> packetId;
		packet >> translation;
		Logger::debug("Received Translation Packet: " + translation.toString() + " for packet id: " + std::to_string(packetId));
		translatedPackets[packetId] = translation;
	});
	registerPacket(NetworkProtocol::S2C_PLAY_MUSIC, [this](sf::Packet& packet) {
		Identifier id;
		float volume, pitch;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		ClientWorld::get().playMusic(id, volume, pitch);
	});
	registerPacket(NetworkProtocol::S2C_PLAY_SOUND, [this](sf::Packet& packet) {
		Identifier id;
		float volume, pitch, x, y, attenuation;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		packet >> x;
		packet >> y;
		packet >> attenuation;
		ClientWorld::get().playSound(id, volume, pitch, {x, y}, attenuation);
	});
	registerPacket(NetworkProtocol::S2C_PLAY_SOUND_3D, [this](sf::Packet& packet) {
		Identifier id;
		float volume, pitch, x, y, z, attenuation;
		packet >> id;
		packet >> volume;
		packet >> pitch;
		packet >> x;
		packet >> y;
		packet >> z;
		packet >> attenuation;
		ClientWorld::get().playSound3d(id, volume, pitch, {x, y, z}, attenuation);
	});
	registerPacket(NetworkProtocol::S2C_LOAD_MAP, [this](sf::Packet& packet) {
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
	});
	registerPacket(NetworkProtocol::S2C_SPAWN_PLAYER, [this](sf::Packet& packet) {
		const auto player = ClientWorld::get().spawnPlayer(packet);
		packet >> player->isClientPlayer;
		GameState::SCORES.emplace(player->getId(), ScoreEntry{player->name, player->colour, player->score, 0, 0});
		GameState::refreshScores();
	});
	registerPacket(NetworkProtocol::S2C_SPAWN_BULLET, [this](sf::Packet& packet) {
		ClientWorld::get().spawnBullet(packet);
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_INPUT, [this](sf::Packet& packet) {
		uint16_t id;
		float x, y;
		bool isFiring;
		packet >> id;
		packet >> x;
		packet >> y;
		packet >> isFiring;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player input for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->input = {x, y};
			player->isFiring = isFiring;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_POSITION_SYNC, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		if (id == this->playerId) {
			packet.clear();
			return;
		}
		float x, y;
		packet >> x >> y;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			entity->setPosition({x, y});
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_BULLET_CHARGE, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		uint16_t charge;
		packet >> charge;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player bullet charge for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->bulletCharge = charge;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_HEALTH, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		uint16_t health;
		packet >> health;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player health for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->health = health;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_DEATH, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player death for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->deathTime = 60;
			player->immunityTime = 120;
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_TELEPORT, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		float x, y;
		packet >> x >> y;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player teleport for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->setPosition({x, y});
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_REMOVE_OBJECT, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		auto iterator = ClientWorld::get().gameObjects.begin();
		while (iterator != ClientWorld::get().gameObjects.end()) {
			if (iterator->first == id) {
				iterator = ClientWorld::get().gameObjects.erase(iterator);
				if (const auto it = GameState::SCORES.find(id); it != GameState::SCORES.end()) {
					GameState::SCORES.erase(it);
					GameState::refreshScores();
				}
			} else {
				++iterator;
			}
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_SCORE, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		uint16_t score;
		packet >> score;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player score for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->score = score;
			if (const auto it = GameState::SCORES.find(player->getId()); it != GameState::SCORES.end()) {
				it->second.score = player->score;
			} else {
				GameState::SCORES.emplace(player->getId(), ScoreEntry{player->name, player->colour, player->score, 0, 0});
			}
			GameState::refreshScores();
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_NAME, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		std::string name;
		packet >> name;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player name for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->name = name;
			if (const auto it = GameState::SCORES.find(player->getId()); it != GameState::SCORES.end()) {
				it->second.name = player->name;
				GameState::refreshScores();
			}
			return;
		}
	});
	registerPacket(NetworkProtocol::S2C_UPDATE_COLOUR, [this](sf::Packet& packet) {
		uint16_t id;
		packet >> id;
		uint8_t r, g, b;
		packet >> r >> g >> b;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getId() != id) continue;
			if (entity->getEntityType() != PlayerEntity::ID) {
				Logger::error("Received player colour for non-player entity");
				return;
			}
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			player->colour = {r, g, b, 255};
			if (const auto it = GameState::SCORES.find(player->getId()); it != GameState::SCORES.end()) {
				it->second.colour = player->colour;
				GameState::refreshScores();
			}
			return;
		}
	});
	registerPacket(NetworkProtocol::PING, [this](sf::Packet& packet) {
		auto pong = NetworkProtocol::PONG.getPacket();
		send(pong);
	});
	registerPacket(NetworkProtocol::PONG, [this](sf::Packet&) {
		this->rtt = this->roundTripTimer.reset().asSeconds();
		Logger::debug("Pong! " + Util::toRoundedString(this->rtt, 2) + " seconds");
	});
}

bool AngleShooterClient::connect(const sf::IpAddress& server) {
	connectingSocket.setBlocking(true);
	const auto status = connectingSocket.connect(server, AngleShooterCommon::PORT, sf::seconds(5.f));
	connectingSocket.setBlocking(false);
	if (status != sf::Socket::Status::Done) {
		connected = false;
		Logger::error("Connection Error: " + Util::toString(status));
		connectingSocket.disconnect();
		return false;
	}
    Logger::info("Connection Status: " + Util::toString(status));
	auto join = NetworkProtocol::C2S_JOIN.getPacket();
	join << OptionsManager::get().getName();
	join << OptionsManager::get().getColour().r;
	join << OptionsManager::get().getColour().g;
	join << OptionsManager::get().getColour().b;
	send(join);
	connected = true;
	return connected;
}

void AngleShooterClient::disconnect() {
	auto quit = NetworkProtocol::C2S_QUIT.getPacket();
	send(quit);
	connectingSocket.disconnect();
}

void AngleShooterClient::run() {
    Logger::debug("Starting AngleShooter Client");
	ClientWorld::get().init();
	sf::Clock deltaClock;
	auto tickTime = 0.;
	auto frameTime = 0.;
	auto secondTime = 0.;
	auto ticks = 0;
	auto frames = 0;
	while (window.isOpen()) {
		const auto deltaTime = deltaClock.restart().asSeconds();
		tickTime += deltaTime;
		frameTime += deltaTime;
		secondTime += deltaTime;
		InputManager::get().handleInput(window);
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
			tickTime = AngleShooterCommon::TIME_PER_TICK;
		}
		if (tickTime >= AngleShooterCommon::TIME_PER_TICK && StateManager::get().getStateId() == GameState::getId() && connected) handleIncomingPackets();
		while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
			tickTime -= AngleShooterCommon::TIME_PER_TICK;
			tick(static_cast<float>(tickTime / AngleShooterCommon::TIME_PER_TICK));
			++ticks;
		}
		if (const auto timePerFrame = OptionsManager::get().getTimePerFrame(); frameTime >= timePerFrame) {
			frameTime -= timePerFrame;
			render(static_cast<float>(tickTime / AngleShooterCommon::TIME_PER_TICK));
			while (frameTime >= timePerFrame) frameTime -= timePerFrame;
			++frames;
		}
		if (secondTime >= .1f) {
			tps = tps * .6 + .4 * (ticks / secondTime);
			fps = fps * .6 + .4 * (frames / secondTime);
			ticks = 0;
			frames = 0;
			secondTime = 0;
		}
	}
	connectingSocket.disconnect();
}

void AngleShooterClient::tick(float deltaTime) {
	AudioManager::get().tick(deltaTime);
	StateManager::get().tick(deltaTime);
}

void AngleShooterClient::render(float deltaTime) {
	window.clear();
	renderTexture.clear();
	StateManager::get().render(deltaTime);
	renderTexture.display();
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setScale({2.f, 2.f});
	window.draw(sprite);
	window.display();
}

void AngleShooterClient::handleIncomingPackets() {
	while (true) {
		sf::Packet packet;
		if (const auto status = connectingSocket.receive(packet); status == sf::Socket::Status::Done) {
			handlePacket(packet);
			continue;
		} else if (status == sf::Socket::Status::Disconnected) {
			this->connected = false;
		} else if (status != sf::Socket::Status::NotReady) {
			Logger::error("Receive Error, disconnecting: " + Util::getAddressString(connectingSocket));
			this->connected = false;
		}
		return;
	}
}

void AngleShooterClient::handlePacket(sf::Packet& packet) {
	uint8_t packetType;
	packet >> packetType;
	if (packetHandlers.contains(packetType)) {
		packetHandlers[packetType](packet);
        if (packet.getReadPosition() != packet.getDataSize()) {
			Logger::error("Packet " + packetIds[packetType].toString() + " has unused data");
			packet.clear();
		}
		Logger::debug("Received packet: " + packetIds[packetType].toString());
		return;
	}
	if (!translatedPackets.contains(packetType)) {
		auto builder = std::stringstream();
		builder << "Received unknown packet id: " << packetType;
		Logger::error(builder.str());
		translatedPackets.emplace(packetType, Identifier("PENDING"));
		auto question = NetworkProtocol::PACKET_QUESTION.getPacket();
		question << packetType;
		send(question);
	} else {
		auto builder = std::stringstream();
		builder << "Received unhandled packet: " << translatedPackets[packetType].toString();
		Logger::error(builder.str());
	}
}

void AngleShooterClient::send(sf::Packet& packet) {
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = connectingSocket.send(packet);
	if (status == sf::Socket::Status::Done) return;
	Logger::error("Send Error: " + Util::toString(status));
	if (status == sf::Socket::Status::Disconnected) {
		this->connected = false;
		if (StateManager::get().getStateId() == GameState::getId()) {
			StateManager::get().pop();
			StateManager::get().push(MenuState::MENU_ID);
		}
	}
}

void AngleShooterClient::registerPacket(const PacketIdentifier& packetType, const std::function<void(sf::Packet& packet)>& handler) {
	this->packetHandlers.emplace(packetType.getId(), handler);
	this->packetIds.emplace(packetType.getId(), packetType);
	Logger::debug("Registered packet: " + packetType.toString() + " (" + std::to_string(packetType.getId()) + ")");
}