#include "PreCompiledServer.h"
#include "ServerPlayerEntity.h"

ServerPlayerEntity::ServerPlayerEntity(uint16_t id, World* world) : PlayerEntity(id, world) {}

void ServerPlayerEntity::tick(float deltaTime) {
	const auto wasDead = this->deathTime > 0;
	PlayerEntity::tick(deltaTime);
	if (wasDead && this->deathTime <= 0) {
		this->setPosition(this->world->getMap()->getRandomSpawnpoint());
		auto teleportPacket = NetworkProtocol::S2C_TELEPORT.getPacket();
		teleportPacket << this->getId() << this->getPosition().x << this->getPosition().y;
		AngleShooterServer::get().sendToAll(teleportPacket);
		this->health = 8;
		auto healthPacket = NetworkProtocol::S2C_HEALTH.getPacket();
		healthPacket << this->getId() << this->health;
		AngleShooterServer::get().sendToAll(healthPacket);
	}
	static Identifier shootSound("bullet.ogg");
	if (isFiring && this->bulletCharge >= 12) {
		this->bulletCharge -= 12;
		auto x = std::cos(this->getRotation().asRadians());
		auto y = std::sin(this->getRotation().asRadians());
		x += Util::randomNormalFloat(0.025f);
		y += Util::randomNormalFloat(0.025f);
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = dynamic_cast<ServerWorld*>(this->world)->spawnBullet(this->getId(), this->getPosition(), velocity * 8.f);
		bullet->setRotation(this->getRotation());
		this->world->playSound(shootSound, .6f, Util::randomFloat(1.f, 1.6f), this->getPosition());
		auto packet = NetworkProtocol::S2C_BULLET_CHARGE.getPacket();
		packet << this->getId() << this->bulletCharge;
		AngleShooterServer::get().sendToAll(packet);
	}
}

bool ServerPlayerEntity::damage(uint16_t source, uint16_t amount) {
	const auto result = PlayerEntity::damage(source, amount);
	if (result) {
		auto packet = NetworkProtocol::S2C_HEALTH.getPacket();
		packet << this->getId() << this->health;
		AngleShooterServer::get().sendToAll(packet);
		static Identifier hurtSound("hurt.ogg");
		this->world->playSound(hurtSound, .8f, Util::randomFloat(0.8f, 1.2f), this->getPosition());
	}
	return result;
}

void ServerPlayerEntity::onDeath(uint16_t source) {
	PlayerEntity::onDeath(source);
	for (auto i = 0; i < 20; i++) {
		const auto x = static_cast<float>(std::sin((18 * i + 25) * (std::numbers::pi / 180)));
		const auto y = static_cast<float>(std::cos((18 * i + 25) * (std::numbers::pi / 180)));
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = dynamic_cast<ServerWorld*>(this->world)->spawnBullet(source, this->getPosition(), velocity * 1.28f);
		bullet->setRotation(sf::radians(std::atan2(velocity.y, velocity.x)));
		this->world->spawnEntity(bullet);
	}
	static Identifier explodeSound("explode.ogg");
	this->world->playSound(explodeSound, .8f, Util::randomFloat(1.2f, 1.8f), this->getPosition());
	auto packet = NetworkProtocol::S2C_DEATH.getPacket();
	packet << this->getId();
	AngleShooterServer::get().sendToAll(packet);
	for (const auto& client : AngleShooterServer::get().clients) {
		if (client->player->getId() != source) continue;
		client->player->score++;
		auto scorePacket = NetworkProtocol::S2C_UPDATE_SCORE.getPacket();
		scorePacket << client->player->getId() << client->player->score;
		AngleShooterServer::get().sendToAll(scorePacket);
		break;
	}
}

bool ServerPlayerEntity::isMarkedForRemoval() const {
	return this->shouldBeErased;
}