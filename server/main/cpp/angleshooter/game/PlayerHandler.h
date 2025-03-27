#pragma once

class PlayerHandler {
public:
	PlayerHandler();
	sf::TcpSocket socket;
	sf::Time lastPacketTime;
	bool ready;
	bool timedOut;
};