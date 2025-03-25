#include "PreCompiledServer.h"
#include "AngleShooterServer.h"

#include "angleshooter/NetworkProtocol.hpp"
#include "game/ServerWorld.h"

AngleShooterServer::AngleShooterServer() : thread(&AngleShooterServer::executionThread, this),
                                           listeningState(false),
                                           clientTimeout(sf::seconds(5.f)),
                                           maxConnectedPlayers(32),
                                           connectedPlayers(0),
                                           peers(1),
                                           waitingThreadEnd(false),
                                            tps(0) {
    listenerSocket.setBlocking(false);
    peers[0].reset(new RemotePeer());
    packetHandlers.emplace(NetworkProtocol::C2S_QUIT.getHash(), [](sf::Packet&, RemotePeer& receivingPeer, bool& detectedTimeout) {
        receivingPeer.timedOut = true;
        detectedTimeout = true;
    });
    packetHandlers.emplace(NetworkProtocol::C2S_PLAYER_EVENT.getHash(), [](sf::Packet& packet, RemotePeer&, bool&) {
        std::int32_t aircraftIdentifier;
        std::int32_t action;
        packet >> aircraftIdentifier >> action;
        // notifyPlayerEvent(aircraftIdentifier, action);
    });
    packetHandlers.emplace(NetworkProtocol::C2S_PLAYER_REALTIME_CHANGE.getHash(), [](sf::Packet& packet, RemotePeer&, bool&) {
        std::int32_t aircraftIdentifier;
        std::int32_t action;
        bool actionEnabled;
        packet >> aircraftIdentifier >> action >> actionEnabled;
        // notifyPlayerRealtimeChange(aircraftIdentifier, action, actionEnabled);
    });
    packetHandlers.emplace(NetworkProtocol::C2S_STATE_UPDATE.getHash(), [](sf::Packet& packet, RemotePeer&, bool&) {
        // std::int32_t numberAircraft;
        // packet >> numberAircraft;
        // for (auto i = 0; i < numberAircraft; ++i) {
            // std::int32_t aircraftIdentifier;
            // std::int32_t aircraftHitpoints;
            // std::int32_t missileAmmo;
            // sf::Vector2f aircraftPosition;
            // packet >> aircraftIdentifier >> aircraftPosition.x >> aircraftPosition.y >> aircraftHitpoints >> missileAmmo;
            // aircraftInfo[aircraftIdentifier].position = aircraftPosition;
            // aircraftInfo[aircraftIdentifier].hitpoints = aircraftHitpoints;
            // aircraftInfo[aircraftIdentifier].missileAmmo = missileAmmo;
        // }
    });
    thread.join();
}

AngleShooterServer::~AngleShooterServer() {
    waitingThreadEnd = true;
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
    ServerWorld::get().init();
    ServerWorld::get().loadMap(Identifier("testmap"));
    sf::Clock deltaClock;
    auto tickTime = 0.;
    auto secondTime = 0.;
    auto ticks = 0;
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

void AngleShooterServer::handleIncomingPackets(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout) {
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
    sf::Packet packet;
    packet << NetworkProtocol::S2C_SPAWN_SELF.getHash();
    // packet << aircraftIdentifierCounter;
    // packet << aircraftInfo[aircraftIdentifierCounter].position.x;
    // packet << aircraftInfo[aircraftIdentifierCounter].position.y;
    // peers[connectedPlayers]->mAircraftIdentifiers.emplace_back(aircraftIdentifierCounter);
    broadcastMessage("New player");
    informWorldState(peers[connectedPlayers]->socket);
    peers[connectedPlayers]->ready = true;
    peers[connectedPlayers]->socket.send(packet);
    peers[connectedPlayers]->lastPacketTime = now();
    connectedPlayers++;
    if (connectedPlayers >= maxConnectedPlayers) {
        setListening(false);
    } else {
        peers.emplace_back(std::make_unique<RemotePeer>());
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
                peers.emplace_back(std::make_unique<RemotePeer>());
                setListening(true);
            }
            broadcastMessage("A player has disconnected");
        } else {
            ++itr;
        }
    }
}

void AngleShooterServer::informWorldState(sf::TcpSocket& socket) {
    sf::Packet packet;
    // packet << static_cast<std::int32_t>(server::PacketType::INITIAL_STATE);
    // // packet << static_cast<std::int32_t>(aircraftCount);
    // for (std::size_t i = 0; i < connectedPlayers; ++i) {
    //     if (peers[i]->ready) {
    //         for (auto identifier : peers[i]->mAircraftIdentifiers) {
    //             // packet << identifier << aircraftInfo[identifier].position.x << aircraftInfo[identifier].position.y << aircraftInfo[identifier].hitpoints << aircraftInfo[identifier].missileAmmo;
    //         }
    //     }
    // }
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

AngleShooterServer::RemotePeer::RemotePeer() : ready(false), timedOut(false) {
    socket.setBlocking(false);
}