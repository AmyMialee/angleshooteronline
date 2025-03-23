#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

AngleShooterServer::AngleShooterServer() : thread(&AngleShooterServer::executionThread, this),
                                       listeningState(false),
                                       clientTimeout(sf::seconds(5.f)),
                                       maxConnectedPlayers(15),
                                       connectedPlayers(0),
                                       peers(1),
                                       aircraftIdentifierCounter(1),
                                       waitingThreadEnd(false)
{
    listenerSocket.setBlocking(false);
    peers[0].reset(new RemotePeer());
}

AngleShooterServer::~AngleShooterServer() {
    waitingThreadEnd = true;
    thread.join();
}

void AngleShooterServer::setListening(bool enable) {
    if (enable) {
        if (!listeningState) listeningState = listenerSocket.listen(AngleShooterCommon::PORT) == sf::TcpListener::Status::Done;
    } else {
        listenerSocket.close();
        listeningState = false;
    }
}

void AngleShooterServer::executionThread() {
    setListening(true);
    const auto tickRate = sf::seconds(1.f / 20.f);
    auto tickTime = sf::Time::Zero;
    sf::Clock tickClock;
    while (!waitingThreadEnd) {
        handleIncomingConnections();
        handleIncomingPackets();
        tickTime += tickClock.getElapsedTime();
        tickClock.restart();
        while (tickTime >= tickRate) {
            tick();
            tickTime -= tickRate;
        }
        sleep(sf::milliseconds(50));
    }
}

void AngleShooterServer::tick() {
    updateClientState();

        // sf::Packet missionSuccessPacket;
        // missionSuccessPacket << static_cast<std::int32_t>(Server::PacketType::kMissionSuccess);
        // sendToAll(missionSuccessPacket);

            //Send the spawn packets to the clients
            // for (std::size_t i = 0; i < enemyCount; ++i) {
            //     sf::Packet packet;
            //     packet << static_cast<std::int32_t>(Server::PacketType::kSpawnEnemy);
            //     packet << 1 + Util::randomInt(static_cast<int>(AircraftType::kAircraftCount) - 1);
            //
            //     packet << worldHeight - battlefieldRect.top + 500;
            //     packet << nextSpawnPosition;
            //
            //     nextSpawnPosition += planeDistance / 2.f;
            //     sendToAll(packet);
            // }
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
            //Interpret the packet and react to it
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

void AngleShooterServer::handleIncomingPackets(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout) {
    std::int32_t packetType;
    packet >> packetType;

    switch (static_cast<client::PacketType> (packetType)) {
        case client::PacketType::QUIT: {
            receivingPeer.timedOut = true;
            detectedTimeout = true;
            break;
        }
        case client::PacketType::PLAYER_EVENT: {
            std::int32_t aircraftIdentifier;
            std::int32_t action;
            packet >> aircraftIdentifier >> action;
            // notifyPlayerEvent(aircraftIdentifier, action);
            break;
        }
        case client::PacketType::PLAYER_REALTIME_CHANGE: {
            std::int32_t aircraftIdentifier;
            std::int32_t action;
            bool actionEnabled;
            packet >> aircraftIdentifier >> action >> actionEnabled;
            // notifyPlayerRealtimeChange(aircraftIdentifier, action, actionEnabled);
            break;
        }
        case client::PacketType::STATE_UPDATE: {
            std::int32_t numberAircraft;
            packet >> numberAircraft;
            for (auto i = 0; i < numberAircraft; ++i) {
                std::int32_t aircraftIdentifier;
                std::int32_t aircraftHitpoints;
                std::int32_t missileAmmo;
                sf::Vector2f aircraftPosition;
                packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y >> aircraftHitpoints >> missileAmmo;
                // aircraftInfo[aircraftIdentifier].position = aircraftPosition;
                // aircraftInfo[aircraftIdentifier].hitpoints = aircraftHitpoints;
                // aircraftInfo[aircraftIdentifier].missileAmmo = missileAmmo;
            }
            break;
        }
    }
}

void AngleShooterServer::handleIncomingConnections() {
    // if (!listeningState) return;
    // if (listenerSocket.accept(peers[connectedPlayers]->socket) != sf::TcpListener::Status::Done) return;
    // //Order the new client to spawn its player 1
    // aircraftInfo[aircraftIdentifierCounter].hitpoints = 100;
    // aircraftInfo[aircraftIdentifierCounter].missileAmmo = 2;
    // sf::Packet packet;
    // packet << static_cast<std::int32_t>(server::PacketType::SPAWN_SELF);
    // packet << aircraftIdentifierCounter;
    // packet << aircraftInfo[aircraftIdentifierCounter].position.x;
    // packet << aircraftInfo[aircraftIdentifierCounter].position.y;
    // peers[connectedPlayers]->mAircraftIdentifiers.emplace_back(aircraftIdentifierCounter);
    // broadcastMessage("New player");
    // informWorldState(peers[connectedPlayers]->socket);
    // notifyPlayerSpawn(aircraftIdentifierCounter++);
    // peers[connectedPlayers]->socket.send(packet);
    // peers[connectedPlayers]->ready = true;
    // peers[connectedPlayers]->lastPacketTime = now();
    // aircraftCount++;
    // connectedPlayers++;
    // if (connectedPlayers >= maxConnectedPlayers) {
    //     setListening(false);
    // } else {
    //     peers.emplace_back(std::make_unique<RemotePeer>());
    // }
}

void AngleShooterServer::handleDisconnections() {
    // for (auto itr = peers.begin(); itr != peers.end();) {
        // if ((*itr)->timedOut) {
            // Inform everyone of a disconnection, erase
            // for (auto identifier : (*itr)->mAircraftIdentifiers) {
                // sendToAll((sf::Packet() << static_cast<std::int32_t>(server::PacketType::PLAYER_DISCONNECT) << identifier));
                // aircraftInfo.erase(identifier);
            // }
            // connectedPlayers--;
            // aircraftCount -= (*itr)->mAircraftIdentifiers.size();
            // itr = peers.erase(itr);
            // If the number of peers has dropped below max_connections
            // if (connectedPlayers < maxConnectedPlayers) {
                // peers.emplace_back(std::make_unique<RemotePeer>());
                // setListening(true);
            // }
            // broadcastMessage("A player has disconnected");
        // } else {
            // ++itr;
        // }
    // }
}

void AngleShooterServer::informWorldState(sf::TcpSocket& socket) {
    sf::Packet packet;
    packet << static_cast<std::int32_t>(server::PacketType::INITIAL_STATE);
    // packet << static_cast<std::int32_t>(aircraftCount);
    for (std::size_t i = 0; i < connectedPlayers; ++i) {
        if (peers[i]->ready) {
            for (auto identifier : peers[i]->mAircraftIdentifiers) {
                // packet << identifier << aircraftInfo[identifier].position.x << aircraftInfo[identifier].position.y << aircraftInfo[identifier].hitpoints << aircraftInfo[identifier].missileAmmo;
            }
        }
    }
    auto status = sf::Socket::Status::Partial;
    while (status == sf::Socket::Status::Partial) status = socket.send(packet);
}

void AngleShooterServer::broadcastMessage(const std::string& message) {
    sf::Packet packet;
    packet << static_cast<std::int32_t>(server::PacketType::BROADCAST_MESSAGE);
    packet << message;
    this->sendToAll(packet);
}

void AngleShooterServer::sendToAll(sf::Packet& packet) {
    for (std::size_t i = 0; i < connectedPlayers; ++i) {
        if (peers[i]->ready) {
            auto status = sf::Socket::Status::Partial;
            while (status == sf::Socket::Status::Partial) status = peers[i]->socket.send(packet);
        }
    }
}

void AngleShooterServer::updateClientState() {
    // sf::Packet updateClientStatePacket;
    // updateClientStatePacket << static_cast<std::int32_t>(server::PacketType::UPDATE_CLIENT_STATE);
    // updateClientStatePacket << static_cast<std::int32_t>(aircraftCount);
    // for (const auto& [first, second] : aircraftInfo) updateClientStatePacket << first << second.position.x << second.position.y << second.hitpoints << second.missileAmmo;
    // sendToAll(updateClientStatePacket);
}

//It is essential to set the sockets to non-blocking - m_socket.setBlocking(false)
//otherwise the server will hang waiting to read input from a connection

AngleShooterServer::RemotePeer::RemotePeer() : ready(false), timedOut(false) {
    socket.setBlocking(false);
}