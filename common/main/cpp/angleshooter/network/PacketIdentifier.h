#pragma once

class PacketIdentifier : public Identifier {
	inline static std::map<uint8_t, PacketIdentifier*> packets;
	inline static uint8_t nextId = 0;
	[[nodiscard]] uint8_t getNextId() const;
	uint8_t id;
	bool reliable;

	PacketIdentifier();
	explicit PacketIdentifier(bool reliable, const std::string& path);
	explicit PacketIdentifier(bool reliable, std::string space, std::string path);

public:
	[[nodiscard]] static PacketIdentifier* of(bool reliable, const std::string& path);
	[[nodiscard]] static PacketIdentifier* of(bool reliable, std::string space, std::string path);
	[[nodiscard]] uint8_t getId() const;
	[[nodiscard]] bool isReliable() const;
	[[nodiscard]] sf::Packet getPacket(NetworkPair* target) const;
	[[nodiscard]] static PacketIdentifier* fromId(uint8_t id);
};
