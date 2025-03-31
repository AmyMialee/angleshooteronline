#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Identifier.h"

Identifier Identifier::empty = Identifier();

Identifier::Identifier() : hash(static_cast<int>(std::hash<std::string>()(this->space + this->path))) {}

Identifier::Identifier(const std::string& path) : Identifier("angleshooter", path) {}

Identifier::Identifier(std::string space, std::string path) :
    space(std::move(space)),
    path(std::move(path)),
    hash(static_cast<int>(std::hash<std::string>()(this->space + this->path))) {}

Identifier Identifier::fromString(const std::string& identifier) {
    const auto pos = identifier.find(':');
    if (pos == std::string::npos) return {"angleshooter", identifier};
    return {identifier.substr(0, pos), identifier.substr(pos + 1)};
}

std::string Identifier::getSpace() const {
    return this->space;
}

std::string Identifier::getPath() const {
    return this->path;
}

int Identifier::getHash() const {
    return this->hash;
}

std::string Identifier::toString() const {
    return this->space + ":" + this->path;
}

sf::Packet Identifier::getPacket() const {
    sf::Packet packet;
    packet << this->hash;
    return packet;
}

bool operator<(const Identifier& leftId, const Identifier& rightId) {
    return leftId.getHash() < rightId.getHash();
}

bool operator==(const Identifier& leftId, const Identifier& rightId) {
    return leftId.getHash() == rightId.getHash();
}

bool operator!=(const Identifier& leftId, const Identifier& rightId) {
    return !(leftId == rightId);
}

sf::Packet& operator<<(sf::Packet& packet, const Identifier& id) {
    const auto string = id.toString();
    packet << string;
    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, Identifier& id) {
    std::string string;
    packet >> string;
    id = Identifier::fromString(string);
    return packet;
}