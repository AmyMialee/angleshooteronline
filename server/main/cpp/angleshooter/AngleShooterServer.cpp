#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

int main() {
	try {
		AngleShooterServer::get().run();
	} catch(std::runtime_error& e) {
		Logger::error(e.what());
	} catch(...) {
		Logger::error("An Error Occurred");
	}
	return 0;
}

AngleShooterServer::AngleShooterServer() {
    NetworkProtocol::initialize();
    listenerSocket.setBlocking(false);
    if (listenerSocket.bind(AngleShooterCommon::PORT) != sf::Socket::Status::Done) throw std::runtime_error("Failed to bind to port, is the server already running?");
	Logger::info("Server started on port " + std::to_string(listenerSocket.getLocalPort()));
	registerPacket(NetworkProtocol::PING, [this](sf::Packet&, NetworkPair* sender) {
        Logger::debug("Ping! from " + sender->getPortedIP()->toString());
        auto pong = NetworkProtocol::PONG->getPacket(sender);
        send(pong, sender);
	});
	registerPacket(NetworkProtocol::PONG, [this](sf::Packet&, NetworkPair* sender) {
		const auto rtt = sender->stopRoundTripTimer();
		Logger::debug("Pong! from " + sender->getPortedIP()->toString() + " in " + Util::toRoundedString(rtt * 1000, 0) + "ms");
	});
	registerPacket(NetworkProtocol::ACK, [this](sf::Packet& packet, NetworkPair* sender) {
		uint32_t sequence;
		packet >> sequence;
		sender->acceptAcknowledgment(sequence);
	});
	registerPacket(NetworkProtocol::CHAT_MESSAGE, [this](sf::Packet& packet, const NetworkPair* sender) {
		std::string message;
		packet >> message;
		Logger::info("Received Chat Message Packet from " + sender->getPortedIP()->toString() + ": " + message);
		for (const auto& client : clients | std::views::values) {
			auto broadcastPacket = NetworkProtocol::CHAT_MESSAGE->getPacket(client);
			broadcastPacket << ("<" + sender->getPortedIP()->toString() + "> " + message);
			send(broadcastPacket, client);
		}
	});
}

void AngleShooterServer::handlePacket(sf::Packet& packet, NetworkPair* sender) {
	sender->resetTimeout();
    uint8_t packetType;
	packet >> packetType;
	if (PacketIdentifier::fromId(packetType)->isReliable()) {
		uint32_t sequence;
		packet >> sequence;
		if (sequence < sender->getAcknowledgedSequence()) {
			auto ack = NetworkProtocol::ACK->getPacket(sender);
			ack << sequence;
			send(ack, sender);
			Logger::debug("Received redundant sequence: " + std::to_string(sequence) + " from " + sender->getPortedIP()->toString());
			return;
		}
		if (sender->setAcknowledgedSequence(sequence)) {
			auto ack = NetworkProtocol::ACK->getPacket(sender);
			ack << sequence;
			send(ack, sender);
		} else {
			Logger::debug("Received premature sequence: " + std::to_string(sequence) + " from " + sender->getPortedIP()->toString());
			return;
		}
	}
    if (packetHandlers.contains(packetType)) {
        packetHandlers[packetType](packet, sender);
        if (packet.getReadPosition() != packet.getDataSize()) {
            Logger::error("Packet " + packetIds[packetType]->toString() + " has unused data");
            packet.clear();
        }
        return;
    }
	Logger::error("Received unknown packet id: " + std::to_string(packetType) + " from " + sender->getPortedIP()->toString());
}

void AngleShooterServer::registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler) {
	this->packetHandlers.emplace(packetType->getId(), handler);
	this->packetIds.emplace(packetType->getId(), packetType);
	Logger::debug("Registered packet: " + packetType->toString() + " (" + std::to_string(packetType->getId()) + ")");
}

void AngleShooterServer::run() {
    Logger::info("Starting AngleShooter Server");
    std::thread receiverThread(&AngleShooterServer::runReceiver, this);
    sf::Clock deltaClock;
    auto tickTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
	auto loops = 0;
    Logger::info("Starting Server Game Loop");
    while (this->running) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        secondTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK) + " ticks (" + Util::toRoundedString(tickTime) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            tickTime -= AngleShooterCommon::TIME_PER_TICK;
            ++ticks;
        }
		++loops;
        if (secondTime >= .1f) {
            tps = ticks / secondTime;
            lps = loops / secondTime;
            tps = tps * .8 + .2 * (ticks / secondTime);
            lps = lps * .8 + .2 * (loops / secondTime);
            ticks = 0;
            loops = 0;
            secondTime = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(6));
    }
}

void AngleShooterServer::runReceiver() {
    Logger::info("Starting Server Network Handler");
	std::unordered_set<PortedIP*> pendingDisconnects;
	while (this->running) {
		std::optional<sf::IpAddress> sender;
		unsigned short port;
		if (sf::Packet packet; listenerSocket.receive(packet, sender, port) == sf::Socket::Status::Done) {
			if (!sender.has_value()) continue;
			auto pip = PortedIP{.ip= sender.value(), .port= port};
			if (!clients.contains(pip)) {
				clients.emplace(pip, new NetworkPair(*this, pip));
				Logger::info("Accepted connection from: " + pip.toString());
			}
			handlePacket(packet, clients[pip]);
			continue;
		}
		for (const auto& client : clients | std::views::values) {
			client->update();
			if (client->shouldDisconnect()) pendingDisconnects.insert(client->getPortedIP());
		}
		auto iterator = clients.begin();
		while (iterator != clients.end()) {
			if (pendingDisconnects.contains(iterator->second->getPortedIP())) {
				delete(iterator->second);
				iterator = clients.erase(iterator);
			} else ++iterator;
		}
		pendingDisconnects.clear();
		sleep(sf::milliseconds(6));
    }
}

void AngleShooterServer::send(sf::Packet& packet, NetworkPair* pair) {
	pair->send(packet);
}

sf::UdpSocket& AngleShooterServer::getSocket() {
	return this->listenerSocket;
}