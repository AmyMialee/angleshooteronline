#pragma once

class AngleShooterClient final {
	std::map<int, std::function<void(sf::Packet& packet)>> packetHandlers;
	std::map<int, Identifier> packetIds;
	std::map<int, Identifier> translatedPackets;
	sf::TcpSocket connectingSocket;
	bool connected = false;

	void tick(float deltaTime);
	void render(float deltaTime);

	void handleIncomingPackets();
	void handlePacket(sf::Packet& packet);
	void registerPacket(const Identifier& packetType, const std::function<void(sf::Packet& packet)>& handler);

protected:
	AngleShooterClient();
	~AngleShooterClient() = default;

public:
	AngleShooterClient(const AngleShooterClient&) = delete;
	void operator=(const AngleShooterClient&) = delete;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	double tps;
	double fps;
	void run();
	void send(sf::Packet& packet);
	bool connect(const sf::IpAddress& server);
	void disconnect();

	static AngleShooterClient& get() {
		static AngleShooterClient instance;
		return instance;
	}
};