#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PacketIdentifier.h"

uint8_t PacketIdentifier::getNextId() const {
	if (nextId == 255) {
		Logger::error("PacketIdentifier ID overflow, unable to allocate more");
		return nextId;
	}
	Logger::debug("Packet Created with ID: " + std::to_string(nextId) + " (" + this->toString() + ")");
	return nextId++;
}

PacketIdentifier::PacketIdentifier() : id(0), reliable(false) {}

PacketIdentifier::PacketIdentifier(const bool reliable, const std::string& path) : Identifier(path), id(getNextId()), reliable(reliable) {}

PacketIdentifier::PacketIdentifier(const bool reliable, std::string space, std::string path) : Identifier(std::move(space), std::move(path)), id(getNextId()), reliable(reliable) {}

PacketIdentifier* PacketIdentifier::of(bool reliable, const std::string& path) {
	auto id = new PacketIdentifier(reliable, path);
	packets.emplace(id->id, id);
	return id;
}

PacketIdentifier* PacketIdentifier::of(bool reliable, std::string space, std::string path) {
	auto id = new PacketIdentifier(reliable, std::move(space), std::move(path));
	packets.emplace(id->id, id);
	return id;
}

uint8_t PacketIdentifier::getId() const {
	return this->id;
}

bool PacketIdentifier::isReliable() const {
	return this->reliable;
}

sf::Packet PacketIdentifier::getPacket() const {
	sf::Packet packet;
	packet << this->id;
	return packet;
}

PacketIdentifier* PacketIdentifier::fromId(uint8_t id) {
	return packets[id];
}