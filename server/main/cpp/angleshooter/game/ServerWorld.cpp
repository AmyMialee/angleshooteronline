#include "PreCompiledServer.h"
#include "ServerWorld.h"

void ServerWorld::playMusic(const Identifier& id, float volume, float pitch) {
	Logger::debug("Sending Music Packet");
	sf::Packet packet;
	packet << NetworkProtocol::S2C_PLAY_MUSIC.getHash();
	packet << id.toString();
	packet << volume;
	packet << pitch;
	AngleShooterServer::get().sendToAll(packet);
}

void ServerWorld::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	Logger::debug("Sending Sound Packet");
	sf::Packet packet;
	packet << NetworkProtocol::S2C_PLAY_SOUND.getHash();
	packet << id.toString();
	packet << volume;
	packet << pitch;
	packet << position.x;
	packet << position.y;
	packet << attenuation;
	AngleShooterServer::get().sendToAll(packet);
}

void ServerWorld::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	Logger::debug("Sending Sound3d Packet");
	sf::Packet packet;
	packet << NetworkProtocol::S2C_PLAY_SOUND_3D.getHash();
	packet << id.toString();
	packet << volume;
	packet << pitch;
	packet << position.x;
	packet << position.y;
	packet << position.z;
	packet << attenuation;
	AngleShooterServer::get().sendToAll(packet);
}

void ServerWorld::loadMap(const Identifier& id) {
	World::loadMap(id);
	Logger::debug("Sending Map Change Packet to " + id.toString());
	sf::Packet packet;
	packet << NetworkProtocol::S2C_LOAD_MAP.getHash();
	packet << id.toString();
	AngleShooterServer::get().sendToAll(packet);
}