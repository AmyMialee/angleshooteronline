#include "PreCompiledServer.h"
#include "ServerWorld.h"

std::shared_ptr<ServerPlayerEntity> ServerWorld::spawnPlayer(ClientConnection& sender) {
	const auto player = std::make_shared<ServerPlayerEntity>(this->getNextId(), this);
	this->spawnEntity(player);
	player->name = sender.name;
	player->setColor(sender.colour);
	player->setPosition(this->getMap()->getRandomSpawnpoint());
	sender.player = player;
	for (const auto& client : AngleShooterServer::get().clients) {
		auto packet = NetworkProtocol::S2C_SPAWN_PLAYER.getPacket();
		player->writeToPacket(packet);
		packet << (client.get() == &sender);
    	AngleShooterServer::get().send(client->socket, packet);
    }
	return player;
}

std::shared_ptr<BulletEntity> ServerWorld::spawnBullet(sf::Color colour, sf::Vector2f position, sf::Vector2f velocity) {
	const auto bullet = std::make_shared<BulletEntity>(this->getNextId(), this);
	bullet->colour = colour;
	bullet->setPosition(position);
	bullet->setVelocity(velocity);
	this->spawnEntity(bullet);
	for (const auto& client : AngleShooterServer::get().clients) {
		auto packet = NetworkProtocol::S2C_SPAWN_BULLET.getPacket();
		bullet->writeToPacket(packet);
		AngleShooterServer::get().send(client->socket, packet);
	}
	return bullet;
}

void ServerWorld::playMusic(const Identifier& id, float volume, float pitch) {
	auto packet = NetworkProtocol::S2C_PLAY_MUSIC.getPacket();
	packet << id;
	packet << volume;
	packet << pitch;
	AngleShooterServer::get().sendToAll(packet);
}

void ServerWorld::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	auto packet = NetworkProtocol::S2C_PLAY_SOUND.getPacket();
	packet << id;
	packet << volume;
	packet << pitch;
	packet << position.x;
	packet << position.y;
	packet << attenuation;
	AngleShooterServer::get().sendToAll(packet);
}

void ServerWorld::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	auto packet = NetworkProtocol::S2C_PLAY_SOUND_3D.getPacket();
	packet << id;
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
	auto packet = NetworkProtocol::S2C_LOAD_MAP.getPacket();
	packet << id;
	AngleShooterServer::get().sendToAll(packet);
}