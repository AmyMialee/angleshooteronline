#pragma once

struct PortedIP {
	sf::IpAddress ip;
	unsigned short port;

	bool operator<(const PortedIP& other) const {
		return std::tie(ip, port) < std::tie(other.ip, other.port);
	}

	friend bool operator==(const PortedIP& first, const PortedIP& second) {
		return first.ip == second.ip && first.port == second.port;
	}

	[[nodiscard]] std::string toString() const {
		return ip.toString() + ":" + std::to_string(port);
	}
};