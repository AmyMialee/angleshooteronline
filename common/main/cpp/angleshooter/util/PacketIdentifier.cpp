#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PacketIdentifier.h"

PacketIdentifier::PacketIdentifier() : id(nextId++) {}

PacketIdentifier::PacketIdentifier(const std::string& path) : Identifier(path), id(nextId++) {}

PacketIdentifier::PacketIdentifier(std::string space, std::string path) : Identifier(std::move(space), std::move(path)), id(nextId++) {}

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