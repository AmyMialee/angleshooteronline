#pragma once

class AngleShooterServer final : public SocketHolder {
	std::map<uint8_t, std::function<void(sf::Packet& packet, NetworkPair* pair)>> packetHandlers;
	std::map<uint8_t, PacketIdentifier*> packetIds;
	std::map<PortedIP, NetworkPair*> clients;
	sf::UdpSocket listenerSocket;
	bool running = true;

	void handlePacket(sf::Packet& packet, NetworkPair* sender);
	void registerPacket(PacketIdentifier* packetType, const std::function<void(sf::Packet& packet, NetworkPair* sender)>& handler);

protected:
	AngleShooterServer();
	~AngleShooterServer() override = default;

public:
	double tps;
	double lps;

	void run();
	void runReceiver();

	void sendToAll(sf::Packet& packet, const std::function<bool(NetworkPair*)>& predicate = [](NetworkPair*) { return true; });
	void send(sf::Packet& packet, NetworkPair* pair);

	sf::UdpSocket& getSocket() override;
	AngleShooterServer(const AngleShooterServer&) = delete;
	void operator=(const AngleShooterServer&) = delete;
	static AngleShooterServer& get() {
		static AngleShooterServer instance;
		return instance;
	}
};