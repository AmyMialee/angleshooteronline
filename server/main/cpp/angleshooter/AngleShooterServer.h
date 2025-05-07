#pragma once

class AngleShooterServer final : public SocketHolder {
	std::map<uint8_t, std::function<void(sf::Packet& packet, const std::unique_ptr<NetworkPair>& pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	std::map<PortedIP, std::unique_ptr<NetworkPair>> clients;
	sf::UdpSocket listenerSocket;
	bool running = true;

	void handlePacket(sf::Packet& packet, const std::unique_ptr<NetworkPair>& sender);
	void registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, const std::unique_ptr<NetworkPair>& sender)>& handler);

protected:
	AngleShooterServer();
	~AngleShooterServer() override = default;

public:
	double tps;
	double lps;

	void run();
	void runReceiver();

	void sendToAll(const sf::Packet& packet, const std::function<bool(const std::unique_ptr<NetworkPair>&)>& predicate = [](const std::unique_ptr<NetworkPair>&) { return true; });
	void send(sf::Packet packet, const std::unique_ptr<NetworkPair>& pair);

	sf::UdpSocket& getSocket() override;
	AngleShooterServer(const AngleShooterServer&) = delete;
	void operator=(const AngleShooterServer&) = delete;
	static AngleShooterServer& get() {
		static AngleShooterServer instance;
		return instance;
	}
};