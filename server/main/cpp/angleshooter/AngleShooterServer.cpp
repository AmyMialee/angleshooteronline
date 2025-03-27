#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

AngleShooterServer* AngleShooterServer::instance = nullptr;

AngleShooterServer::AngleShooterServer() :
    listeningState(false),
    clientTimeout(sf::seconds(5.f)),
    maxConnectedPlayers(32),
    connectedPlayers(0),
    peers(1),
    waitingThreadEnd(false),
    tps(0) {
    instance = this;
    listenerSocket.setBlocking(false);
    peers[0].reset(new PlayerHandler());
    packetHandlers.emplace(NetworkProtocol::C2S_QUIT.getHash(), [](sf::Packet&, PlayerHandler& receivingPeer, bool& detectedTimeout) {
        receivingPeer.timedOut = true;
        detectedTimeout = true;
    });
}

void AngleShooterServer::setListening(bool enable) {
    if (enable) {
        if (!listeningState) listeningState = listenerSocket.listen(AngleShooterCommon::PORT) == sf::TcpListener::Status::Done;
    } else {
        listenerSocket.close();
        listeningState = false;
    }
}

void AngleShooterServer::run() {
    setListening(true);
    ServerWorld::get().init();
    ServerWorld::get().loadMap(Identifier("testmaplarge"));
    sf::Clock deltaClock;
    auto tickTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
    Logger::debug("Server Started");
    while (!waitingThreadEnd) {
        const auto deltaTime = deltaClock.restart().asSeconds();
        tickTime += deltaTime;
        secondTime += deltaTime;
        if (tickTime > 1.) {
            Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / AngleShooterCommon::TIME_PER_TICK, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
            tickTime = AngleShooterCommon::TIME_PER_TICK;
        }
        if (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            handleIncomingConnections();
            handleIncomingPackets();
        }
        while (tickTime >= AngleShooterCommon::TIME_PER_TICK) {
            tickTime -= AngleShooterCommon::TIME_PER_TICK;
            tick(static_cast<float>(tickTime / AngleShooterCommon::TIME_PER_TICK));
            ++ticks;
        }
        if (secondTime >= .1f) {
            tps = tps * .6 + .4 * (ticks / secondTime);
            ticks = 0;
            secondTime = 0;
        }
    }
}

void AngleShooterServer::tick(float deltaTime) {
    ServerWorld::get().tick(deltaTime);
    updateClientState();
}

sf::Time AngleShooterServer::now() const {
    return clock.getElapsedTime();
}

void AngleShooterServer::handleIncomingPackets() {
    auto detectedTimeout = false;
    for (auto& peer : peers) {
        if (!peer->ready) continue;
        sf::Packet packet;
        while (peer->socket.receive(packet) == sf::Socket::Status::Done) {
            handleIncomingPackets(packet, *peer, detectedTimeout);
            peer->lastPacketTime = now();
            packet.clear();
        }
        if (now() > peer->lastPacketTime + clientTimeout) {
            peer->timedOut = true;
            detectedTimeout = true;
        }
    }
    if (detectedTimeout) handleDisconnections();
}

void AngleShooterServer::handleIncomingPackets(sf::Packet& packet, PlayerHandler& receivingPeer, bool& detectedTimeout) {
    int packetType;
    packet >> packetType;
    if (packetHandlers.contains(packetType)) {
        packetHandlers[packetType](packet, receivingPeer, detectedTimeout);
        Logger::info("Received packet type: " + std::to_string(packetType));
        return;
    }
    auto builder = std::stringstream();
    builder << "Received unknown packet type: " << packetType;
    if (const auto address = receivingPeer.socket.getRemoteAddress(); address.has_value()) {
        builder << " from " << address.value();
    } else {
        builder << " from unknown address";
    }
    Logger::error(builder.str());
}

void AngleShooterServer::handleIncomingConnections() {
    if (!listeningState) return;
    if (listenerSocket.accept(peers[connectedPlayers]->socket) != sf::TcpListener::Status::Done) return;
    if (!peers[connectedPlayers]->socket.getRemoteAddress().has_value()) {
        Logger::error("Failed to get remote address");
        return;
    }
    broadcastMessage("New player");
    initialSetup(peers[connectedPlayers]->socket);
    peers[connectedPlayers]->ready = true;
    peers[connectedPlayers]->lastPacketTime = now();
    connectedPlayers++;
    if (connectedPlayers >= maxConnectedPlayers) {
        setListening(false);
    } else {
        peers.emplace_back(std::make_unique<PlayerHandler>());
    }
}

void AngleShooterServer::handleDisconnections() {
    for (auto itr = peers.begin(); itr != peers.end();) {
        if ((*itr)->timedOut) {
            // for (auto identifier : (*itr)->mAircraftIdentifiers) {
                // sendToAll((sf::Packet() << static_cast<std::int32_t>(server::PacketType::PLAYER_DISCONNECT) << identifier));
                // aircraftInfo.erase(identifier);
            // }
            connectedPlayers--;
            // aircraftCount -= (*itr)->mAircraftIdentifiers.size();
            itr = peers.erase(itr);
            if (connectedPlayers < maxConnectedPlayers) {
                peers.emplace_back(std::make_unique<PlayerHandler>());
                setListening(true);
            }
            broadcastMessage("A player has disconnected");
        } else {
            ++itr;
        }
    }
}

void AngleShooterServer::initialSetup(sf::TcpSocket& socket) {
    Logger::debug("Sending Initial Setup Packet to " + socket.getRemoteAddress().value().toString());  // NOLINT(bugprone-unchecked-optional-access) Already checked earlier
    sf::Packet packet;
    packet << NetworkProtocol::S2C_INITIAL_SETUP.getHash();
    packet << ServerWorld::get().getMap()->getId();
    auto status = sf::Socket::Status::Partial;
    while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void AngleShooterServer::broadcastMessage(const std::string& message) {
    sf::Packet packet;
    packet << NetworkProtocol::S2C_BROADCAST_MESSAGE.getHash();
    packet << message;
    this->sendToAll(packet);
}

void AngleShooterServer::sendToAll(sf::Packet& packet) {
    for (std::size_t i = 0; i < connectedPlayers; ++i) if (peers[i]->ready) {
        auto status = sf::Socket::Status::Partial;
        while (status == sf::Socket::Status::Partial) status = peers[i]->socket.send(packet);
    }
}

void AngleShooterServer::updateClientState() {
    
}

AngleShooterServer* AngleShooterServer::get() {
    return instance;
}