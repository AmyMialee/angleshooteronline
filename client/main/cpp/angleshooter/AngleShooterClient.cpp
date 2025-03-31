#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

int main(int, char*[]) {
	try {
		AngleShooterClient app;
		app.run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterClient::AngleShooterClient() {
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
	});
	registerPacket(NetworkProtocol::S2C_BROADCAST_MESSAGE, [this](sf::Packet& packet) {
		std::string message;
		packet >> message;
		Logger::debug("Received Broadcast Message Packet from server: " + message);
	});
	registerPacket(NetworkProtocol::PACKET_TRANSLATION, [&](sf::Packet& packet) {
		int packetId;
		packet >> packetId;
		Identifier translation;
		packet >> translation;
		Logger::debug("Received Translation Packet: " + translation.toString() + " for packet id: " + std::to_string(packetId));
		translatedPackets[packetId] = translation;
	});
}

bool AngleShooterClient::connect(const sf::IpAddress& server) {
	const auto status = connectingSocket.connect(server, AngleShooterCommon::PORT);
	connected = status == sf::Socket::Status::Done;
	auto join = NetworkProtocol::C2S_JOIN.getPacket();
	join << "ClientName";
	send(join);
	return connected;
}

void AngleShooterClient::disconnect() {
	auto quit = NetworkProtocol::C2S_QUIT.getPacket();
	send(quit);
	connectingSocket.disconnect();
}

void AngleShooterClient::run() {
	connect(sf::IpAddress(127, 0, 0, 1));
    Logger::debug("Starting AngleShooter Client");
	sf::Clock deltaClock;
	auto tickTime = 0.;
	while (true) {
		const auto deltaTime = deltaClock.restart().asSeconds();
		tickTime += deltaTime;
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
			tickTime = AngleShooterCommon::TIME_PER_TICK;
		}
		if (tickTime >= AngleShooterCommon::TIME_PER_TICK) handleIncomingPackets();
		while (tickTime >= AngleShooterCommon::TIME_PER_TICK) tickTime -= AngleShooterCommon::TIME_PER_TICK;
	}
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