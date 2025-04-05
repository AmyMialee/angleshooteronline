#include <PreCompiledClient.h>
#include "ClientWorld.h"

ClientWorld::ClientWorld() : mapRenderer() {}

std::shared_ptr<ClientPlayerEntity> ClientWorld::spawnPlayer(sf::Packet& packet) {
	uint16_t id;
	packet >> id;
	const auto player = std::make_shared<ClientPlayerEntity>(id, this);
	player->readFromPacket(packet);
	this->spawnEntity(player);
	return player;
}

std::shared_ptr<BulletEntity> ClientWorld::spawnBullet(sf::Packet& packet) {
	uint16_t id;
	packet >> id;
	const auto bullet = std::make_shared<BulletEntity>(id, this);
	bullet->readFromPacket(packet);
	this->spawnEntity(bullet);
	return bullet;
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