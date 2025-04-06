#pragma once

class AngleShooterServer final {
	std::map<uint8_t, std::function<void(ClientConnection& sender, sf::Packet& packet)>> packetHandlers;
	std::map<uint8_t, Identifier> packetIds;

	sf::TcpListener listenerSocket;
	std::unordered_set<ClientConnection*> pendingDisconnects;

	std::queue<std::pair<sf::TcpSocket*, sf::Packet>> packetQueue;
	std::mutex packetLock;

	void handleIncomingClients();
	void handleDisconnectingClients();
	void handleIncomingPackets();
	void handlePacket(ClientConnection& sender, sf::Packet& packet);
	void registerPacket(const PacketIdentifier& packetType, const std::function<void(ClientConnection& sender, sf::Packet& packet)>& handler);

protected:
	AngleShooterServer();
	~AngleShooterServer() = default;

public:
	std::vector<std::unique_ptr<ClientConnection>> clients;
	double tps;

	AngleShooterServer(const AngleShooterServer&) = delete;
	void operator=(const AngleShooterServer&) = delete;
	void run();
	void runReceiver();
	void runSender();

	void sendToAll(sf::Packet& packet);
	void send(ClientConnection& player, sf::Packet& packet);
	void send(sf::TcpSocket& socket, sf::Packet& packet);

	static AngleShooterServer& get() {
		static AngleShooterServer instance;
		return instance;
	}
};
