#include <PreCompiledClient.h>
#include "ClientWorld.h"

ClientWorld::ClientWorld() : mapRenderer() {}

void ClientWorld::spawnPlayer(const std::string& name, int color, sf::Vector2f position, bool isClientPlayer) {
	const auto player = std::make_shared<ClientPlayerEntity>(this, name, isClientPlayer);
	player->setColor(color);
	player->setPosition(position);
	this->spawnEntity(player);
}

void ClientWorld::spawnBullet(int color, sf::Vector2f position, sf::Vector2f velocity) {
	const auto bullet = std::make_shared<BulletEntity>(this, color, position, velocity);
	bullet->setPosition(position);
	this->spawnEntity(bullet);
}

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