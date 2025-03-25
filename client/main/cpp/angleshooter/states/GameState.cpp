#include "PreCompiledClient.h"
#include "GameState.h"

#include "angleshooter/NetworkProtocol.hpp"

const Identifier GameState::GAME_ID("game");

GameState::GameState() : State(),
	connected(false),
	activeState(true),
	gameStarted(false),
	clientTimeout(5.f),
	timeSinceLastPacket(0.f)
{
	if (const sf::IpAddress ip(127, 0, 0, 1); socket.connect(ip, AngleShooterCommon::PORT, sf::seconds(5.f)) == sf::TcpSocket::Status::Done) {
		connected = true;
	} else {
		failedConnectionClock.restart();
	}
	socket.setBlocking(false);

	packetHandlers.emplace(NetworkProtocol::S2C_BROADCAST_MESSAGE.getHash(), [this](int, sf::Packet& packet) {
		std::string message;
		packet >> message;
		broadcasts.push_back(message);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_SPAWN_SELF.getHash(), [this](int packetType, sf::Packet& packet) {
		// sf::Int32 aircraft_identifier;
		// sf::Vector2f aircraft_position;
		// packet >> aircraft_identifier >> aircraft_position.x >> aircraft_position.y;
		// Aircraft* aircraft = m_world.AddAircraft(aircraft_identifier);
		// aircraft->setPosition(aircraft_position);
		// m_players[aircraft_identifier].reset(new Player(&socket, aircraft_identifier, GetContext().keys1));
		// m_local_player_identifiers.push_back(aircraft_identifier);
		gameStarted = true;
	});
	// packetHandlers.emplace(NetworkProtocol::S2C_PLAYER_CONNECT.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 aircraft_identifier;
	// 	sf::Vector2f aircraft_position;
	// 	packet >> aircraft_identifier >> aircraft_position.x >> aircraft_position.y;
	// 	Aircraft* aircraft = m_world.AddAircraft(aircraft_identifier);
	// 	aircraft->setPosition(aircraft_position);
	// 	m_players[aircraft_identifier].reset(new Player(&socket, aircraft_identifier, nullptr));
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_PLAYER_DISCONNECT.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 aircraft_identifier;
	// 	packet >> aircraft_identifier;
	// 	m_world.RemoveAircraft(aircraft_identifier);
	// 	m_players.erase(aircraft_identifier);
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_INITIAL_STATE.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 aircraft_count;
	// 	float world_height, current_scroll;
	// 	packet >> world_height >> current_scroll;
	// 	m_world.SetWorldHeight(world_height);
	// 	m_world.SetCurrentBattleFieldPosition(current_scroll);
	// 	packet >> aircraft_count;
	// 	for (sf::Int32 i = 0; i < aircraft_count; ++i) {
	// 		sf::Int32 aircraft_identifier;
	// 		sf::Int32 hitpoints;
	// 		sf::Int32 missile_ammo;
	// 		sf::Vector2f aircraft_position;
	// 		packet >> aircraft_identifier >> aircraft_position.x >> aircraft_position.y >> hitpoints >> missile_ammo;
	// 		Aircraft* aircraft = m_world.AddAircraft(aircraft_identifier);
	// 		aircraft->setPosition(aircraft_position);
	// 		aircraft->SetHitpoints(hitpoints);
	// 		aircraft->SetMissileAmmo(missile_ammo);
	// 		m_players[aircraft_identifier].reset(new Player(&socket, aircraft_identifier, nullptr));
	// 	}
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_PLAYER_EVENT.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 aircraftIdentifier;
	// 	sf::Int32 action;
	// 	packet >> aircraftIdentifier >> action;
	// 	auto itr = m_players.find(aircraft_identifier);
	// 	if (itr != m_players.end()) {
	// 		itr->second->HandleNetworkEvent(static_cast<Action>(action), m_world.GetCommandQueue());
	// 	}
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_PLAYER_REALTIME_CHANGE.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 aircraft_identifier;
	// 	sf::Int32 action;
	// 	bool action_enabled;
	// 	packet >> aircraft_identifier >> action >> action_enabled;
	// 	auto itr = m_players.find(aircraft_identifier);
	// 	if (itr != m_players.end()) {
	// 		itr->second->HandleNetworkRealtimeChange(static_cast<Action>(action), action_enabled);
	// 	}
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_SPAWN_ENEMY.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	float height;
	// 	sf::Int32 type;
	// 	float relative_x;
	// 	packet >> type >> height >> relative_x;
	// 	m_world.AddEnemy(static_cast<AircraftType>(type), relative_x, height);
	// 	m_world.SortEnemies();
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_MISSION_SUCCESS.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	RequestStackPush(StateID::kMissionSuccess);
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_SPAWN_PICKUP.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	sf::Int32 type;
	// 	sf::Vector2f position;
	// 	packet >> type >> position.x >> position.y;
	// 	m_world.CreatePickup(position, static_cast<PickupType>(type));
	// });
	// packetHandlers.emplace(NetworkProtocol::S2C_UPDATE_CLIENT_STATE.getHash(), [this](uint16_t packetType, sf::Packet& packet) {
	// 	float current_world_position;
	// 	sf::Int32 aircraft_count;
	// 	packet >> current_world_position >> aircraft_count;
	// 	float current_view_position = m_world.GetViewBounds().top + m_world.GetViewBounds().height;
	// 	m_world.SetWorldScrollCompensation(current_view_position / current_world_position);
	// 	for (sf::Int32 i = 0; i < aircraft_count; ++i) {
	// 		sf::Vector2f aircraft_position;
	// 		sf::Int32 aircraft_identifier;
	// 		sf::Int32 hitpoints;
	// 		sf::Int32 ammo;
	// 		packet >> aircraft_identifier >> aircraft_position.x >> aircraft_position.y >> hitpoints >> ammo;
	// 		Aircraft* aircraft = m_world.GetAircraft(aircraft_identifier);
	// 		bool is_local_plane = std::find(m_local_player_identifiers.begin(), m_local_player_identifiers.end(), aircraft_identifier) != m_local_player_identifiers.end();
	// 		if (aircraft && !is_local_plane) {
	// 			sf::Vector2f interpolated_position = aircraft->getPosition() + (aircraft_position - aircraft->getPosition()) * 0.1f;
	// 			aircraft->setPosition(interpolated_position);
	// 			aircraft->SetHitpoints(hitpoints);
	// 			aircraft->SetMissileAmmo(ammo);
	// 		}
	// 	}
	// });
}

void GameState::init() {
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientWorld::get().init();
	AudioManager::get().playMusic(GAME_MUSIC);
	activeState = true;
}

void GameState::destroy() {
	if (!connected) return;
	sf::Packet packet;
	packet << NetworkProtocol::C2S_QUIT.getHash();
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void GameState::loadAssets() {
	TextureHolder::getInstance().load(Map::DEFAULT_TILE);
	TextureHolder::getInstance().load(Identifier("cobble.png"));
	TextureHolder::getInstance().load(Identifier("player.png"));
	TextureHolder::getInstance().load(Identifier("player2.png"));
	TextureHolder::getInstance().load(Identifier("bullet.png"));
	TextureHolder::getInstance().load(Identifier("bullet2.png"));
	SoundHolder::getInstance().load(Identifier("bullet.ogg"));
	SoundHolder::getInstance().load(Identifier("hurt.ogg"));
	SoundHolder::getInstance().load(Identifier("explode.ogg"));
}

void GameState::render(float deltaTime) {
	if (connected) {
		auto& window = *ClientContext::get()->getRenderTexture();
		window.setView(window.getDefaultView());
		WorldRenderer::get().render(deltaTime);
		ClientContext::get()->getWindow()->setView(ClientContext::get()->getWindow()->getDefaultView());
		// if (!broadcasts.empty()) {
			// ClientContext::get()->getWindow()->draw(m_broadcast_text);
		// }
		// if (m_local_player_identifiers.size() < 2 && playerInvitationTime < sf::seconds(0.5f)) {
			// ClientContext::get()->getWindow()->draw(m_player_invitation_text);
		// }
	} else {
		// ClientContext::get()->getWindow()->draw(m_failed_connection_text);
	}
}

bool GameState::shouldRenderNextState() const {
	return false;
}

bool GameState::tick(float deltaTime) {
	if (connected) {
		ClientWorld::get().tick(deltaTime);
		if (sf::Packet packet; socket.receive(packet) == sf::Socket::Status::Done) {
			timeSinceLastPacket = 0;
			int packetType;
			packet >> packetType;
			handlePacket(packetType, packet);
		} else {
			if (timeSinceLastPacket > clientTimeout) {
				connected = false;
				failedConnectionClock.restart();
			}
		}

		sf::Packet update;
		update << NetworkProtocol::C2S_STATE_UPDATE.getHash();
		auto status = sf::Socket::Status::Partial;
		while (status == sf::Socket::Status::Partial) status = socket.send(update);
		timeSinceLastPacket = 0;

		updateBroadcastMessage(deltaTime);
		timeSinceLastPacket += deltaTime;
	} else if (failedConnectionClock.getElapsedTime() >= sf::seconds(5.f)) {
		requestStackClear();
		requestStackPush(MenuState::getId());
	}
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) if (keyPressed->scancode == sf::Keyboard::Scan::Escape) requestStackPush(PauseState::getId());
	return false;
}

Identifier GameState::getId() {
	return GAME_ID;
}

Identifier GameState::getStateId() {
	return GAME_ID;
}

void GameState::updateBroadcastMessage(float deltaTime) {
	if (broadcasts.empty()) return;
	// broadcastElapsedTime += deltaTime;
	// if (broadcastElapsedTime <= 2.f) return;
	// broadcasts.erase(broadcasts.begin());
	// if (!broadcasts.empty()) broadcastElapsedTime = 0;
}

void GameState::handlePacket(int packetType, sf::Packet& packet) {
	if (packetHandlers.contains(packetType)) {
		packetHandlers[packetType](packetType, packet);
        Logger::info("Received packet type: " + std::to_string(packetType));
		return;
	}
	auto builder = std::stringstream();
	builder << "Received unknown packet type: " << packetType;
	Logger::error(builder.str());
}