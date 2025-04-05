#pragma once
#include "ServerPlayerEntity.h"

class ClientConnection {
public:
	ClientConnection();
	sf::TcpSocket socket;
	std::string name = "Anon";
	sf::Color colour = {0xFF, 0xAA, 0xAA, 0xFF};
	std::map<int, Identifier> translatedPackets;
	std::shared_ptr<ServerPlayerEntity> player = nullptr;
};
