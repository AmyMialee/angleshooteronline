#pragma once

class AngleShooterClient final {
	std::map<int, std::function<void(sf::Packet& packet)>> packetHandlers;
	std::map<int, Identifier> packetIds;
	std::map<int, Identifier> translatedPackets;
	
	sf::TcpSocket connectingSocket;
	bool connected = false;

	bool connect(const sf::IpAddress& server);
	void disconnect();
	void handleIncomingPackets();
	void handlePacket(sf::Packet& packet);
	void registerPacket(const Identifier& packetType, const std::function<void(sf::Packet& packet)>& handler);

public:
	AngleShooterClient();
	void run();
	void send(sf::Packet& packet);
};