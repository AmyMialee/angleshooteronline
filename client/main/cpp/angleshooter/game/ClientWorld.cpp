#include <PreCompiledClient.h>
#include "ClientWorld.h"

ClientWorld::ClientWorld() : mapRenderer() {}

void ClientWorld::playMusic(const Identifier& id, float volume, float pitch) {
	AudioManager::get().playMusic(id, volume, pitch);
}

void ClientWorld::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	AudioManager::get().playSound(id, volume, pitch, position, attenuation);
}

void ClientWorld::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	AudioManager::get().playSound3d(id, volume, pitch, position, attenuation);
}

void ClientWorld::loadMap(const Identifier& id) {
	World::loadMap(id);
	mapRenderer = new MapRenderer(*getMap());
}