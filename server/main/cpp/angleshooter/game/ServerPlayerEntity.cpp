#include "PreCompiledServer.h"
#include "ServerPlayerEntity.h"

ServerPlayerEntity::ServerPlayerEntity(uint16_t id, World* world) : PlayerEntity(id, world) {}

void ServerPlayerEntity::tick(float deltaTime) {
	PlayerEntity::tick(deltaTime);
	static Identifier shootSound("bullet.ogg");
	if (isFiring && this->bulletCharge >= 12) {
		this->bulletCharge -= 12;
		auto x = std::cos(this->getRotation().asRadians());
		auto y = std::sin(this->getRotation().asRadians());
		x += Util::randomNormalFloat(0.025f);
		y += Util::randomNormalFloat(0.025f);
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = dynamic_cast<ServerWorld*>(this->world)->spawnBullet(this->color, this->getPosition(), velocity * 8.f);
		bullet->setRotation(this->getRotation());
		this->world->playSound(shootSound, .6f, Util::randomFloat(1.f, 1.6f));
		auto packet = NetworkProtocol::S2C_BULLET_CHARGE.getPacket();
		packet << this->getId() << this->bulletCharge;
		AngleShooterServer::get().sendToAll(packet);
	}
}

void ServerPlayerEntity::onDeath(sf::Color sourceColour) {
	PlayerEntity::onDeath(sourceColour);
	for (auto i = 0; i < 20; i++) {
		const auto x = static_cast<float>(std::sin((18 * i + 25) * (std::numbers::pi / 180)));
		const auto y = static_cast<float>(std::cos((18 * i + 25) * (std::numbers::pi / 180)));
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = dynamic_cast<ServerWorld*>(this->world)->spawnBullet(sourceColour, this->getPosition(), velocity * 1.28f);
		bullet->setRotation(sf::radians(std::atan2(velocity.y, velocity.x)));
		this->world->spawnEntity(bullet);
	}
	static Identifier explodeSound("explode.ogg");
	this->world->playSound(explodeSound, .8f, Util::randomFloat(1.2f, 1.8f));
}

bool ServerPlayerEntity::isMarkedForRemoval() const {
	return this->shouldBeErased;
}
