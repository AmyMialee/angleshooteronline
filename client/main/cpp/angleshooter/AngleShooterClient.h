#pragma once

class AngleShooterClient final : public SocketHolder {
	std::map<uint8_t, std::function<void(sf::Packet& packet, NetworkPair* pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	sf::UdpSocket socket;
	bool running = true;

	void handlePacket(sf::Packet& packet, NetworkPair* sender);
	void registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler);

protected:
	AngleShooterClient();
	~AngleShooterClient() override = default;

public:
	NetworkPair* server = nullptr;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	double fps;
	double tps;
	double lps;

	void run();
	void tick();
	void render(float deltaTime);
	void runReceiver();

	void send(sf::Packet& packet);
	void connect(PortedIP& server);

	sf::UdpSocket& getSocket() override;
	AngleShooterClient(const AngleShooterClient&) = delete;
	void operator=(const AngleShooterClient&) = delete;
	static AngleShooterClient& get() {
		static AngleShooterClient instance;
		return instance;
	}
};