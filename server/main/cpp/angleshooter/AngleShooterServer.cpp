#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

int main(int, char*[]) {
    try {
        AngleShooterServer app;
        app.run();
    } catch(std::runtime_error& e) {
        Logger::error(e.what());
    } catch(...) {
        Logger::error("An Error Occurred");
    }
    return 0;
}

AngleShooterServer::AngleShooterServer() {
    if (listenerSocket.listen(AngleShooterCommon::PORT) != sf::Socket::Status::Done) {
        throw std::runtime_error("Failed to bind to port, is the server already running?");
    }
    socketSelector.add(listenerSocket);
    // TODO: Register Packets
}

void AngleShooterServer::run() {
    Logger::debug("Starting AngleShooter Server");
    sf::Clock deltaClock;
    auto tickTime = 0.;
    while (true) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        if (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            if (socketSelector.isReady(listenerSocket)) {
                handleIncomingClients();
            } else {
                handleIncomingPackets();
            }
            handleDisconnectingClients();
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) tickTime -= AngleShooterCommon::TIME_PER_TICK;
    }
}

void AngleShooterServer::registerPacket(const Identifier& packetType, std::function<void(ClientConnection& sender, sf::Packet& packet)> handler) {
    this->packetHandlers.emplace(packetType.getHash(), handler);
    this->packetIds.emplace(packetType.getHash(), packetType);
}

void AngleShooterServer::handleIncomingClients() {
    if (auto join = std::make_unique<ClientConnection>(); listenerSocket.accept(join->socket) == sf::Socket::Status::Done) {
        socketSelector.add(join->socket);
        clients.push_back(std::move(join));
        sf::Packet joinPacket;
        joinPacket << NetworkProtocol::S2C_ANNOUNCE_JOIN << ++nextClientId;
        sendToAll(joinPacket);
    }
}

void AngleShooterServer::handleDisconnectingClients() {
    auto iterator = clients.begin();
    while (iterator != clients.end()) {
        if (const auto client = iterator->get(); !client->socket.getRemoteAddress().has_value()) {
            sf::Packet leavePacket;
            leavePacket << NetworkProtocol::S2C_ANNOUNCE_LEAVE;
            for (auto& other : clients) {
                if (other.get() != client) sendTo(other->socket, leavePacket);
            }
            socketSelector.remove(client->socket);
            iterator = clients.erase(iterator);
        } else {
            ++iterator;
        }
    }
}

void AngleShooterServer::handleIncomingPackets() {
    for (auto& client : clients) {
        if (!socketSelector.isReady(client->socket)) continue;
        sf::Packet packet; 
        if (client->socket.receive(packet) != sf::Socket::Status::Done) continue;
        handlePacket(*client, packet);
    }
}

void AngleShooterServer::handlePacket(ClientConnection& sender, sf::Packet& packet) {
    int packetType;
    packet >> packetType;
    if (packetHandlers.contains(packetType)) {
        packetHandlers[packetType](sender, packet);
        auto builder = std::stringstream();
        builder << "Received Packet: " << this->packetIds[packetType].toString();
        if (const auto address = sender.socket.getRemoteAddress(); address.has_value()) {
            builder << " from " << address.value();
        } else {
            builder << " from unknown address";
        }
        Logger::debug(builder.str());
        return;
    }
    auto builder = std::stringstream();
    builder << "Received unknown packet id: " << packetType;
    if (const auto address = sender.socket.getRemoteAddress(); address.has_value()) {
        builder << " from " << address.value();
    } else {
        builder << " from unknown address";
    }
    Logger::error(builder.str());
}

void AngleShooterServer::sendToAll(sf::Packet& packet) {
    for (const auto& client : clients) sendTo(client->socket, packet);
}

void AngleShooterServer::sendTo(ClientConnection& player, sf::Packet& packet) {
    this->sendTo(player.socket, packet);
}

void AngleShooterServer::sendTo(sf::TcpSocket& socket, sf::Packet& packet) {
    auto status = sf::Socket::Status::Partial;
    while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}