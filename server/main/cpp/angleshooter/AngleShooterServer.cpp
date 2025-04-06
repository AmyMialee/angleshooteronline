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
        uint8_t r, g, b;
        packet >> name;
        packet >> r >> g >> b;
        auto endName = name;
        auto count = 0;
        for (const auto& client : clients) if (client->name == endName) endName = name + " " + std::to_string(++count);
        Logger::debug("Received Join Packet from " + endName + " (" + Util::getAddressString(sender.socket) + ")");
        sender.name = endName;
        sender.colour = {r, g, b, 255};
        auto mapPacket = NetworkProtocol::S2C_INITIAL_SETUP.getPacket();
        mapPacket << ServerWorld::get().getMap()->getId();
        sender.player = ServerWorld::get().spawnPlayer(sender);
        mapPacket << sender.player->getId();
        send(sender.socket, mapPacket);
        for (auto& client : clients) {
            if (client.get()->player->getId() == sender.player->getId()) continue;
            auto syncPlayerPacket = NetworkProtocol::S2C_SPAWN_PLAYER.getPacket();
            client.get()->player->writeToPacket(syncPlayerPacket);
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
        syncPlayerPacket << sender.player->getId();
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        syncPlayerPacket << isFiring;
        for (auto& client : clients) {
            if (client.get()->player->getId() == sender.player->getId()) continue;
            send(*client, syncPlayerPacket);
        }
    });
    registerPacket(NetworkProtocol::C2S_PLAYER_POSITION_SYNC, [this](const ClientConnection& sender, sf::Packet& packet) {
        float x, y;
        packet >> x;
        packet >> y;
        sender.player->setPosition({x, y});
        auto syncPlayerPacket = NetworkProtocol::S2C_PLAYER_POSITION_SYNC.getPacket();
        syncPlayerPacket << sender.player->getId();
        syncPlayerPacket << x;
        syncPlayerPacket << y;
        for (auto& client : clients) {
            if (client.get()->player->getId() == sender.player->getId()) continue;
            send(*client, syncPlayerPacket);
        }
    });
}

void AngleShooterServer::run() {
    Logger::debug("Starting AngleShooter Server");
    ServerWorld::get().init();
    ServerWorld::get().loadMap(Identifier("testmaplarge"));
    std::thread receiverThread(&AngleShooterServer::runReceiver, this);
    std::thread senderThread(&AngleShooterServer::runSender, this);
    sf::Clock deltaClock;
    auto tickTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
    Logger::debug("Starting Server Game Loop");
    while (true) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        secondTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            tickTime -= AngleShooterCommon::TIME_PER_TICK;
            ServerWorld::get().tick(deltaTime);
            ++ticks;
        }
        if (secondTime >= .1f) {
            tps = tps * .6 + .4 * (ticks / secondTime);
            ticks = 0;
            secondTime = 0;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
}

void AngleShooterServer::runReceiver() {
    Logger::debug("Starting Server Network Handler");
    while (true) {
        handleIncomingClients();
        handleIncomingPackets();
        handleDisconnectingClients();
        std::this_thread::sleep_for(std::chrono::milliseconds(12));
    }
}

void AngleShooterServer::runSender() {
    Logger::debug("Starting Server Packet Sender");
    while (true) {
        std::unique_lock lock(packetLock);
        if (!packetQueue.empty()) {
            auto [socket, packet] = packetQueue.front();
            packetQueue.pop();
            lock.unlock();
            auto status = sf::Socket::Status::Partial;
            while (status == sf::Socket::Status::Partial) status = socket->send(packet);
            if (status != sf::Socket::Status::Done) Logger::error("Send Error: " + Util::getAddressString(*socket));
        } else {
            lock.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(12));
        }
    }
}

void AngleShooterServer::handleIncomingClients() {
    while (true) {
        auto join = std::make_unique<ClientConnection>();
        join->socket.setBlocking(false);
        if (const auto status = listenerSocket.accept(join->socket); status == sf::Socket::Status::Done) {
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
            iterator->get()->player->shouldBeErased = true;
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
        if (packet.getReadPosition() != packet.getDataSize()) {
            Logger::error("Packet " + packetIds[packetType].toString() + " has unused data");
            packet.clear();
        }
        Logger::debug("Received Packet: " + packetIds[packetType].toString() + " from " + sender.name + " (" + Util::getAddressString(sender.socket) + ")");
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
    std::lock_guard lock(packetLock);
    packetQueue.emplace(&socket, packet);
}

void AngleShooterServer::registerPacket(const Identifier& packetType, const std::function<void(ClientConnection& sender, sf::Packet& packet)>& handler) {
    this->packetHandlers.emplace(packetType.getHash(), handler);
    this->packetIds.emplace(packetType.getHash(), packetType.toString());
    Logger::debug("Registered packet: " + packetType.toString() + " (" + std::to_string(packetType.getHash()) + ")");
}