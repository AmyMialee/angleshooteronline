#pragma once

class AngleShooterClient final {
	std::map<uint8_t, std::function<void(sf::Packet& packet)>> packetHandlers;
	std::map<uint8_t, Identifier> packetIds;
	std::map<uint8_t, Identifier> translatedPackets;
	sf::TcpSocket connectingSocket;

	void tick(float deltaTime);
	void render(float deltaTime);

	void handleIncomingPackets();
	void handlePacket(sf::Packet& packet);
	void registerPacket(const PacketIdentifier& packetType, const std::function<void(sf::Packet& packet)>& handler);

protected:
	AngleShooterClient();
	~AngleShooterClient() = default;

public:
	uint16_t playerId;
	sf::IpAddress foundAddress;
	AngleShooterClient(const AngleShooterClient&) = delete;
	void operator=(const AngleShooterClient&) = delete;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	double tps;
	double fps;
	bool connected = false;
	void run();
	void send(sf::Packet& packet);
	bool connect(const sf::IpAddress& server);
	void disconnect();

	static AngleShooterClient& get() {
		static AngleShooterClient instance;
		return instance;
	}
};