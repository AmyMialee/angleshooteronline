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

PacketIdentifier::PacketIdentifier() : id(0) {}

PacketIdentifier::PacketIdentifier(const std::string& path) : Identifier(path), id(getNextId()) {}

PacketIdentifier::PacketIdentifier(std::string space, std::string path) : Identifier(std::move(space), std::move(path)), id(getNextId()) {}

PacketIdentifier PacketIdentifier::fromString(const std::string& identifier) {
	const auto pos = identifier.find(':');
	if (pos == std::string::npos) return {"angleshooter", identifier};
	return {identifier.substr(0, pos), identifier.substr(pos + 1)};
}

uint8_t PacketIdentifier::getId() const {
	return this->id;
}

sf::Packet PacketIdentifier::getPacket() const {
	sf::Packet packet;
	packet << this->id;
	return packet;
}