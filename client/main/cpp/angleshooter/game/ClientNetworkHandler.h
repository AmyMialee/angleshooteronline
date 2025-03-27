#pragma once

class ClientNetworkHandler : public Singleton<ClientNetworkHandler> {
	friend class Singleton<ClientNetworkHandler>;
	ClientNetworkHandler();
	
public:
	void init();
	void destroy();
	void render(float deltaTime);
	void tick(float deltaTime);
	void send(sf::Packet& packet);
	void updateBroadcastMessage(float deltaTime);
	void handlePacket(int packetType, sf::Packet& packet);
	std::map<int, std::function<void(int packet_type, sf::Packet& packet)>> packetHandlers;
	sf::TcpSocket socket;
	bool connected;
	std::vector<std::string> broadcasts;
	sf::Clock failedConnectionClock;
	float clientTimeout;
	float timeSinceLastPacket;
};
