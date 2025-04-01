#pragma once

class AngleShooterServer final {
	std::map<int, std::function<void(ClientConnection& sender, sf::Packet& packet)>> packetHandlers;
	std::map<int, Identifier> packetIds;

	sf::TcpListener listenerSocket;
	std::vector<std::unique_ptr<ClientConnection>> clients;
	std::unordered_set<ClientConnection*> pendingDisconnects;
	std::uint8_t nextClientId = 0;

	void handleIncomingClients();
	void handleDisconnectingClients();
	void handleIncomingPackets();
	void handlePacket(ClientConnection& sender, sf::Packet& packet);
	void registerPacket(const Identifier& packetType, const std::function<void(ClientConnection& sender, sf::Packet& packet)>& handler);

protected:
	AngleShooterServer();
	~AngleShooterServer() = default;

public:
	AngleShooterServer(const AngleShooterServer&) = delete;
	void operator=(const AngleShooterServer&) = delete;
	void run();
	void sendToAll(sf::Packet& packet);
	void send(ClientConnection& player, sf::Packet& packet);
	void send(sf::TcpSocket& socket, sf::Packet& packet);

	static AngleShooterServer& get() {
		static AngleShooterServer instance;
		return instance;
	}
};