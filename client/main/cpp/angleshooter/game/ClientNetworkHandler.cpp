#include "PreCompiledClient.h"
#include "ClientNetworkHandler.h"

ClientNetworkHandler::ClientNetworkHandler() :
	connected(false),
	clientTimeout(5.f),
	timeSinceLastPacket(0.f) {
	socket.setBlocking(false);
	packetHandlers.emplace(NetworkProtocol::S2C_BROADCAST_MESSAGE.getHash(), [this](int, sf::Packet& packet) {
		std::string message;
		packet >> message;
		broadcasts.push_back(message);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_INITIAL_SETUP.getHash(), [this](int, sf::Packet& packet) {
		Identifier map;
		packet >> map;
		ClientWorld::get().loadMap(map);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_LOAD_MAP.getHash(), [this](int, sf::Packet& packet) {
		Identifier id;
		packet >> id;
		ClientWorld::get().loadMap(id);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_PLAY_MUSIC.getHash(), [this](int, sf::Packet& packet) {
		Identifier id;
		float volume, pitch;
		packet >> id >> volume >> pitch;
		ClientWorld::get().playMusic(id, volume, pitch);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_PLAY_SOUND.getHash(), [this](int, sf::Packet& packet) {
		Identifier id;
		float volume, pitch, attenuation;
		sf::Vector2f position;
		packet >> id >> volume >> pitch >> position.x >> position.y >> attenuation;
		ClientWorld::get().playSound(id, volume, pitch, position, attenuation);
	});
	packetHandlers.emplace(NetworkProtocol::S2C_PLAY_SOUND_3D.getHash(), [this](int, sf::Packet& packet) {
		Identifier id;
		float volume, pitch, attenuation;
		sf::Vector3f position;
		packet >> id >> volume >> pitch >> position.x >> position.y >> position.z >> attenuation;
		ClientWorld::get().playSound3d(id, volume, pitch, position, attenuation);
	});
}

void ClientNetworkHandler::init() {
	const auto ip = sf::IpAddress::getLocalAddress().value();
	const auto status = socket.connect(ip, AngleShooterCommon::PORT, sf::seconds(5.f));
	Logger::debug("Init Socket Status: " + Util::toString(status));
	if (status == sf::TcpSocket::Status::Done) {
		connected = true;
	} else {
		failedConnectionClock.restart();
	}
}

void ClientNetworkHandler::destroy() {
	if (!connected) return;
	sf::Packet packet;
	packet << NetworkProtocol::C2S_QUIT.getHash();
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void ClientNetworkHandler::render(float deltaTime) {
	if (connected) {
		auto& window = *ClientContext::get()->getRenderTexture();
		window.setView(window.getDefaultView());
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

void ClientNetworkHandler::tick(float deltaTime) {
	if (connected) {
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

		// sf::Packet update;
		// update << NetworkProtocol::C2S_STATE_UPDATE.getHash();
		// auto status = sf::Socket::Status::Partial;
		// while (status == sf::Socket::Status::Partial) status = socket.send(update);
		timeSinceLastPacket = 0;

		updateBroadcastMessage(deltaTime);
		timeSinceLastPacket += deltaTime;
	} else if (failedConnectionClock.getElapsedTime() >= sf::seconds(5.f)) {
		StateManager::get().clear();
		StateManager::get().push(MenuState::getId());
	}
}

void ClientNetworkHandler::send(sf::Packet& packet) {
	auto status = sf::Socket::Status::Partial;
	while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void ClientNetworkHandler::updateBroadcastMessage(float deltaTime) {
	if (broadcasts.empty()) return;
	// broadcastElapsedTime += deltaTime;
	// if (broadcastElapsedTime <= 2.f) return;
	// broadcasts.erase(broadcasts.begin());
	// if (!broadcasts.empty()) broadcastElapsedTime = 0;
}

void ClientNetworkHandler::handlePacket(int packetType, sf::Packet& packet) {
	if (packetHandlers.contains(packetType)) {
		packetHandlers[packetType](packetType, packet);
		Logger::info("Received packet type: " + std::to_string(packetType));
		return;
	}
	auto builder = std::stringstream();
	builder << "Received unknown packet type: " << packetType;
	Logger::error(builder.str());
}