#pragma once

class ClientConnection {
public:
	ClientConnection();
	sf::TcpSocket socket;
	std::string name = "Anon";
	int colour = 0xFFFFFFFF;
	std::map<int, Identifier> translatedPackets;
	std::shared_ptr<PlayerEntity> player = nullptr;
};