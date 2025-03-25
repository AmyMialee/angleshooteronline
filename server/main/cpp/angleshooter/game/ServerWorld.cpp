#include "PreCompiledServer.h"
#include "ServerWorld.h"

void ServerWorld::playMusic(const Identifier& id, float volume, float pitch) {
	//TODO: Send sound packet
	Logger::info("Sending Sound Packet");
}

void ServerWorld::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	//TODO: Send sound packet
	Logger::info("Sending Sound Packet");
}

void ServerWorld::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	//TODO: Send sound packet
	Logger::info("Sending Sound Packet");
}