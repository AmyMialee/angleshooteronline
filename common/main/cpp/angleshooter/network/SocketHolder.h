#pragma once

class SocketHolder {
public:
	virtual ~SocketHolder() = default;

	virtual sf::UdpSocket& getSocket() = 0;
};