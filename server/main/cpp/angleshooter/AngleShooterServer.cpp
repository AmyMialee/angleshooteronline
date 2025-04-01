#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

int main(int, char*[]) {
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
    listenerSocket.setBlocking(false);
    if (listenerSocket.listen(AngleShooterCommon::PORT) != sf::Socket::Status::Done) throw std::runtime_error("Failed to bind to port, is the server already running?");
    registerPacket(NetworkProtocol::C2S_JOIN, [this](ClientConnection& sender, sf::Packet& packet) {
        std::string name;
        int colour;
        packet >> name;
        packet >> colour;
        auto endName = name;
        auto count = 0;
        for (const auto& client : clients) if (client->name == endName) endName = name + " " + std::to_string(++count);
        Logger::debug("Received Join Packet from " + endName + " (" + Util::getAddressString(sender.socket) + ")");
        sender.name = endName;
        auto mapPacket = NetworkProtocol::S2C_INITIAL_SETUP.getPacket();
        mapPacket << ServerWorld::get().getMap()->getId();
        send(sender.socket, mapPacket);
        sender.player = ServerWorld::get().spawnPlayer(sender);
        for (auto& client : clients) {
            if (client.get() == &sender) continue;
            auto syncPlayerPacket = NetworkProtocol::S2C_SPAWN_PLAYER.getPacket();
            syncPlayerPacket << client->name;
            syncPlayerPacket << client->colour;
            syncPlayerPacket << client->player->getPosition().x;
            syncPlayerPacket << client->player->getPosition().y;
            syncPlayerPacket << false;
            send(sender.socket, syncPlayerPacket);
        }
    });
    registerPacket(NetworkProtocol::C2S_CHANGE_NAME, [this](ClientConnection& sender, sf::Packet& packet) {
        std::string name;
        packet >> name;
        auto endName = name;
        auto count = 0;
        for (const auto& client : clients) if (client->name == endName) endName = name + " " + std::to_string(++count);
        Logger::debug("Received Change Name Packet " + endName + " (" + Util::getAddressString(sender.socket) + "), previously " + sender.name);
        sender.name = endName;
    });
    registerPacket(NetworkProtocol::C2S_SEND_MESSAGE, [this](const ClientConnection& sender, sf::Packet& packet) {
        std::string message;
        packet >> message;
        Logger::debug("Received Send Message Packet from " + sender.name + " (" + Util::getAddressString(sender.socket) + "): " + message);
        auto broadcastPacket = NetworkProtocol::S2C_BROADCAST_MESSAGE.getPacket();
        broadcastPacket << "<" << sender.name << ">: " << message;
        sendToAll(broadcastPacket);
    });
    registerPacket(NetworkProtocol::C2S_QUIT, [this](ClientConnection& sender, sf::Packet& packet) {
        Logger::debug("Received Quit Packet from " + sender.name + " (" + Util::getAddressString(sender.socket) + ")");
        sender.socket.disconnect();
    });
    registerPacket(NetworkProtocol::PACKET_QUESTION, [this](ClientConnection& sender, sf::Packet& packet) {
        int packetId;
        packet >> packetId;
        auto translation = NetworkProtocol::PACKET_TRANSLATION.getPacket();
        translation << packetId;
        translation << this->packetIds[packetId];
        Logger::debug("Received Translation Question Packet: " + this->packetIds[packetId].toString() + " for packet id: " + std::to_string(packetId));
        send(sender, translation);
    });
    registerPacket(NetworkProtocol::PACKET_TRANSLATION, [this](ClientConnection& sender, sf::Packet& packet) {
        int packetId;
        Identifier translation;
        packet >> packetId;
        packet >> translation;
        Logger::debug("Received Translation Packet from " + sender.name + " (" + Util::getAddressString(sender.socket) + "): " + translation.toString());
        sender.translatedPackets[packetId] = translation;
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_INPUT, [this](const ClientConnection& sender, sf::Packet& packet) {
        float x, y;
        bool isFiring;
        packet >> x;
        packet >> y;
        packet >> isFiring;
        sender.player->input = {x, y};
        sender.player->isFiring = isFiring;
        auto syncPlayerPacket = NetworkProtocol::S2C_PLAYER_INPUT.getPacket();
        syncPlayerPacket << sender.name;
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        syncPlayerPacket << isFiring;
        for (auto& client : clients) {
            if (client.get() == &sender) continue;
            send(*client, syncPlayerPacket);
        }
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_POSITION_SYNC, [this](const ClientConnection& sender, sf::Packet& packet) {
        float x, y;
        packet >> x;
        packet >> y;
        sender.player->setPosition({x, y});
    });
}

void AngleShooterServer::run() {
    Logger::debug("Starting AngleShooter Server");
    ServerWorld::get().init();
    ServerWorld::get().loadMap(Identifier("testmaplarge"));
    std::thread networkThread(&AngleShooterServer::runReceiver, this);
    sf::Clock deltaClock;
    auto tickTime = 0.;
    while (true) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            ServerWorld::get().tick(deltaTime);
            tickTime -= AngleShooterCommon::TIME_PER_TICK;
        }
    }
}

void AngleShooterServer::runReceiver() {
    while (true) {
        handleIncomingClients();
        handleIncomingPackets();
        handleDisconnectingClients();
    }
}

void AngleShooterServer::handleIncomingClients() {
    while (true) {
        auto join = std::make_unique<ClientConnection>();
        join->socket.setBlocking(false);
        if (const auto status = listenerSocket.accept(join->socket); status == sf::Socket::Status::Done) {
            auto joinPacket = NetworkProtocol::S2C_ANNOUNCE_JOIN.getPacket();
            joinPacket << ++nextClientId;
            sendToAll(joinPacket);
            clients.push_back(std::move(join));
        } else {
            if (status != sf::Socket::Status::NotReady) Logger::error("Join Error: " + Util::getAddressString(join->socket));
            break;
        }
    }
}

void AngleShooterServer::handleIncomingPackets() {
    for (auto& client : clients) {
        sf::Packet packet;
        if (const auto status = client->socket.receive(packet); status == sf::Socket::Status::Done) {
            handlePacket(*client, packet);
        } else if (status == sf::Socket::Status::Disconnected) {
            pendingDisconnects.insert(client.get());
        } else if (status != sf::Socket::Status::NotReady) {
            Logger::error("Receive Error: " + Util::getAddressString(client->socket));
        }
    }
}

void AngleShooterServer::handleDisconnectingClients() {
    auto iterator = clients.begin();
    while (iterator != clients.end()) {
        if (pendingDisconnects.contains(iterator->get())) {
            auto joinPacket = NetworkProtocol::S2C_ANNOUNCE_LEAVE.getPacket();
            joinPacket << ++nextClientId;
            sendToAll(joinPacket);
            iterator = clients.erase(iterator);
        } else ++iterator;
    }
    pendingDisconnects.clear();
}

void AngleShooterServer::handlePacket(ClientConnection& sender, sf::Packet& packet) {
    int packetType;
    packet >> packetType;
    if (packetHandlers.contains(packetType)) {
        packetHandlers[packetType](sender, packet);
        // auto builder = std::stringstream();
        // builder << "Received Packet: " << this->packetIds[packetType].toString();
        // if (const auto address = sender.socket.getRemoteAddress(); address.has_value()) {
        //     builder << " from " << address.value();
        // } else {
        //     builder << " from unknown address";
        // }
        // Logger::debug(builder.str());
        return;
    }
    if (!sender.translatedPackets.contains(packetType)) {
        auto builder = std::stringstream();
        builder << "Received unknown packet id: " << packetType;
        if (const auto address = sender.socket.getRemoteAddress(); address.has_value()) {
            builder << " from " << address.value();
        } else {
            builder << " from unknown address";
        }
        Logger::error(builder.str());
        sender.translatedPackets.emplace(packetType, Identifier("PENDING"));
        auto question = NetworkProtocol::PACKET_QUESTION.getPacket();
        question << packetType;
        send(sender, question);
    } else {
        auto builder = std::stringstream();
        builder << "Received unhandled packet: " << sender.translatedPackets[packetType].toString();
        if (const auto address = sender.socket.getRemoteAddress(); address.has_value()) {
            builder << " from " << address.value();
        } else {
            builder << " from unknown address";
        }
        Logger::error(builder.str());
    }
}

void AngleShooterServer::sendToAll(sf::Packet& packet) {
    for (const auto& client : clients) send(client->socket, packet);
}

void AngleShooterServer::send(ClientConnection& player, sf::Packet& packet) {
    this->send(player.socket, packet);
}

void AngleShooterServer::send(sf::TcpSocket& socket, sf::Packet& packet) {
    auto status = sf::Socket::Status::Partial;
    while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void AngleShooterServer::registerPacket(const Identifier& packetType, const std::function<void(ClientConnection& sender, sf::Packet& packet)>& handler) {
    this->packetHandlers.emplace(packetType.getHash(), handler);
    this->packetIds.emplace(packetType.getHash(), packetType.toString());
    Logger::debug("Registered packet: " + packetType.toString() + " (" + std::to_string(packetType.getHash()) + ")");
}