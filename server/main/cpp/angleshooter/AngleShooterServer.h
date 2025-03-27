#pragma once

class AngleShooterServer final {
	std::map<int, std::function<void(ClientConnection& sender, sf::Packet& packet)>> packetHandlers;
	std::map<int, Identifier> packetIds;

	sf::TcpListener listenerSocket;
    sf::SocketSelector socketSelector;
	
	std::vector<std::unique_ptr<ClientConnection>> clients;
	std::uint8_t nextClientId = 0;

	void registerPacket(const Identifier& packetType, std::function<void(ClientConnection& sender, sf::Packet& packet)> handler);
	void handleIncomingClients();
	void handleDisconnectingClients();
	void handleIncomingPackets();
	void handlePacket(ClientConnection& sender, sf::Packet& packet);

public:
	AngleShooterServer();
	void run();
	void sendToAll(sf::Packet& packet);
	void sendTo(ClientConnection& player, sf::Packet& packet);
	void sendTo(sf::TcpSocket& socket, sf::Packet& packet);
};