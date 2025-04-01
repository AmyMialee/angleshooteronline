#pragma once

class Identifier {
	std::string space;
	std::string path;
	int hash;

public:
	static Identifier empty;
	Identifier();
	explicit Identifier(const std::string& path);
	Identifier(std::string space, std::string path);
	static Identifier fromString(const std::string& identifier);
	[[nodiscard]] std::string getSpace() const;
	[[nodiscard]] std::string getPath() const;
	[[nodiscard]] int getHash() const;
	[[nodiscard]] std::string toString() const;
	[[nodiscard]] sf::Packet getPacket() const;
	friend bool operator<(const Identifier& leftId, const Identifier& rightId);
	friend bool operator==(const Identifier& leftId, const Identifier& rightId);
	friend bool operator!=(const Identifier& leftId, const Identifier& rightId);
	friend sf::Packet& operator<<(sf::Packet& packet, const Identifier& id);
	friend sf::Packet& operator>>(sf::Packet& packet, Identifier& id);
};