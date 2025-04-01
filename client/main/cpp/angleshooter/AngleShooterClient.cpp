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
	window(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Titlebar | sf::Style::Close),
	renderTexture({960, 540}),
	tps(static_cast<int>(1 / AngleShooterCommon::TIME_PER_TICK)),
	fps(144)
{
	window.clear();
	window.setKeyRepeatEnabled(false);
	StateManager::get().push(SplashState::getId());
	OptionsManager::get().loadFromFile();
	connectingSocket.setBlocking(false);
	registerPacket(NetworkProtocol::S2C_ANNOUNCE_JOIN, [this](sf::Packet& packet) {
		std::string name;
		packet >> name;
		Logger::debug("Received Join Packet, name: " + name);
	});
	registerPacket(NetworkProtocol::S2C_ANNOUNCE_LEAVE, [this](sf::Packet& packet) {
		std::string name;
		packet >> name;
		Logger::debug("Received Leave Packet, name: " + name);
	});
	registerPacket(NetworkProtocol::S2C_INITIAL_SETUP, [this](sf::Packet& packet) {
		Logger::debug("Received Initial Setup Packet");
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
	});
	registerPacket(NetworkProtocol::S2C_BROADCAST_MESSAGE, [this](sf::Packet& packet) {
		std::string message;
		packet >> message;
		Logger::debug("Received Broadcast Message Packet from server: " + message);
	});
	registerPacket(NetworkProtocol::PACKET_QUESTION, [this](sf::Packet& packet) {
		int packetId;
		packet >> packetId;
		auto translation = NetworkProtocol::PACKET_TRANSLATION.getPacket();
		translation << packetId;
		translation << this->packetIds[packetId];
		Logger::debug("Received Translation Question Packet: " + this->packetIds[packetId].toString() + " for packet id: " + std::to_string(packetId));
		send(translation);
	});
	registerPacket(NetworkProtocol::PACKET_TRANSLATION, [this](sf::Packet& packet) {
		int packetId;
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
		std::string name;
		int colour;
		float x, y;
		bool isClientPlayer;
		packet >> name;
		packet >> colour;
		packet >> x;
		packet >> y;
		packet >> isClientPlayer;
		Logger::debug("Received Spawn Player Packet: " + name + " (colour: " + std::to_string(colour) + ") at (" + std::to_string(x) + ", " + std::to_string(y) + "), " + std::to_string(isClientPlayer));
		ClientWorld::get().spawnPlayer(name, colour, {x, y}, isClientPlayer);
	});
	registerPacket(NetworkProtocol::S2C_SPAWN_BULLET, [this](sf::Packet& packet) {
		int colour;
		float x, y;
		float xVel, yVel;
		packet >> colour;
		packet >> x;
		packet >> y;
		packet >> xVel;
		packet >> yVel;
		ClientWorld::get().spawnBullet(colour, {x, y}, {xVel, yVel});
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_INPUT, [this](sf::Packet& packet) {
		std::string name;
		float x, y;
		bool isFiring;
		packet >> name;
		packet >> x;
		packet >> y;
		packet >> isFiring;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getEntityType() != PlayerEntity::ID) continue;
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			if (player->getName() != name) continue;
			player->input = {x, y};
			player->isFiring = isFiring;
		}
	});
	registerPacket(NetworkProtocol::S2C_PLAYER_POSITION_SYNC, [this](sf::Packet& packet) {
		std::string name;
		float x, y;
		packet >> name;
		packet >> x;
		packet >> y;
		for (const auto& entity : ClientWorld::get().getEntities()) {
			if (entity->getEntityType() != PlayerEntity::ID) continue;
			const auto player = dynamic_cast<PlayerEntity*>(entity.get());
			if (player->getName() != name) continue;
			player->setPosition({x, y});
		}
	});
}

bool AngleShooterClient::connect(const sf::IpAddress& server) {
	const auto status = connectingSocket.connect(server, AngleShooterCommon::PORT);
	auto join = NetworkProtocol::C2S_JOIN.getPacket();
	join << OptionsManager::get().getName();
	join << OptionsManager::get().getColour();
	send(join);
	connected = static_cast<int>(status) < 3;
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
	sf::Packet packet;
	if (const auto status = connectingSocket.receive(packet); status == sf::Socket::Status::Done) {
		handlePacket(packet);
	} else if (status == sf::Socket::Status::Disconnected) {
		this->connected = false;
	} else if (status != sf::Socket::Status::NotReady) {
		Logger::error("Receive Error: " + Util::getAddressString(connectingSocket));
	}
}

void AngleShooterClient::handlePacket(sf::Packet& packet) {
	int packetType;
	packet >> packetType;
	if (packetHandlers.contains(packetType)) {
		packetHandlers[packetType](packet);
		Logger::debug("Received Packet: " + this->packetIds[packetType].toString());
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
}

void AngleShooterClient::registerPacket(const Identifier& packetType, const std::function<void(sf::Packet& packet)>& handler) {
	this->packetHandlers.emplace(packetType.getHash(), handler);
	this->packetIds.emplace(packetType.getHash(), packetType);
	Logger::debug("Registered packet: " + packetType.toString() + " (" + std::to_string(packetType.getHash()) + ")");
}