#pragma once

class AngleShooterServer final {
	static AngleShooterServer* instance;
	std::map<int, std::function<void(sf::Packet& packet, PlayerHandler& receivingPeer, bool& detectedTimeout)>> packetHandlers;
	sf::TcpListener listenerSocket;
	sf::Clock clock;
	bool listeningState;
	sf::Time clientTimeout;
	std::uint16_t maxConnectedPlayers;
	std::uint16_t connectedPlayers;
	std::vector<std::unique_ptr<PlayerHandler>> peers;
	bool waitingThreadEnd;
	double tps;


	void setListening(bool enable);
	void tick(float deltaTime);
	[[nodiscard]] sf::Time now() const;

	void handleIncomingPackets();
	void handleIncomingPackets(sf::Packet& packet, PlayerHandler& receivingPeer, bool& detectedTimeout);

	void handleIncomingConnections();
	void handleDisconnections();
	
	void initialSetup(sf::TcpSocket& socket);
	void updateClientState();

public:
	explicit AngleShooterServer();
	void run();
	AngleShooterServer(const AngleShooterServer&) = delete;
	AngleShooterServer& operator=(const AngleShooterServer&) = delete;
	void broadcastMessage(const std::string& message);
	void sendToAll(sf::Packet& packet);
	static AngleShooterServer* get();
};