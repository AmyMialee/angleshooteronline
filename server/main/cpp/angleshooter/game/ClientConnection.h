#pragma once

class ClientConnection {
public:
	ClientConnection();
	sf::TcpSocket socket;
	std::string name = "Anon";
	std::map<int, Identifier> translatedPackets;
};