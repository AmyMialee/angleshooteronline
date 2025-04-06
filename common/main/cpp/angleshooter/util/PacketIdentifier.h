#pragma once

class PacketIdentifier : public Identifier {
	inline static uint8_t nextId = 0;
	uint8_t id;
	[[nodiscard]] uint8_t getNextId() const;

public:
	PacketIdentifier();
	explicit PacketIdentifier(const std::string& path);
	PacketIdentifier(std::string space, std::string path);
	static PacketIdentifier fromString(const std::string& identifier);
	[[nodiscard]] uint8_t getId() const;
	[[nodiscard]] sf::Packet getPacket() const;
};
