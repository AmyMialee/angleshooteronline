#pragma once

class AngleShooterServer {
	struct RemotePeer {
		RemotePeer();
		sf::TcpSocket socket;
		sf::Time lastPacketTime;
		std::vector<std::int32_t> mAircraftIdentifiers;
		bool ready;
		bool timedOut;
	};

	typedef std::unique_ptr<RemotePeer> PeerPtr;

	std::thread thread;
	sf::Clock clock;
	sf::TcpListener listenerSocket;
	bool listeningState;
	sf::Time clientTimeout;

	std::size_t maxConnectedPlayers;
	std::size_t connectedPlayers;

	// std::size_t aircraftCount;
	// std::map<std::int32_t, AircraftInfo> aircraftInfo;

	std::vector<PeerPtr> peers;
	std::int32_t aircraftIdentifierCounter;
	bool waitingThreadEnd;

	void setListening(bool enable);
	void executionThread();
	void tick();
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