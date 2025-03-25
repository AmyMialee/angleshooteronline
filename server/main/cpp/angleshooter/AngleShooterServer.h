#pragma once

class AngleShooterServer {
	struct RemotePeer {
		RemotePeer();
		sf::TcpSocket socket;
		sf::Time lastPacketTime;
		bool ready;
		bool timedOut;
	};

	std::map<int, std::function<void(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout)>> packetHandlers;

	std::thread thread;
	sf::TcpListener listenerSocket;
	sf::Clock clock;
	bool listeningState;
	sf::Time clientTimeout;

	std::uint16_t maxConnectedPlayers;
	std::uint16_t connectedPlayers;

	std::vector<std::unique_ptr<RemotePeer>> peers;
	bool waitingThreadEnd;

	double tps;

	void setListening(bool enable);
	void executionThread();
	void tick(float deltaTime);
	[[nodiscard]] sf::Time now() const;

	void handleIncomingPackets();
	void handleIncomingPackets(sf::Packet& packet, RemotePeer& receivingPeer, bool& detectedTimeout);

	void handleIncomingConnections();
	void handleDisconnections();

	void informWorldState(sf::TcpSocket& socket);
	void broadcastMessage(const std::string& message);
	void sendToAll(sf::Packet& packet);
	void updateClientState();

public:
	explicit AngleShooterServer();
	~AngleShooterServer();
};