#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "BulletEntity.h"

BulletEntity::BulletEntity(uint16_t id, World* world) : Entity(id, world) {
	this->setDrag(0.f);
	this->setScale({6, 6});
}

const Identifier& BulletEntity::getEntityType() const {
	return ID;
}

void BulletEntity::tick(float deltaTime) {
	Entity::tick(deltaTime);
}

void BulletEntity::onCollision(Entity& other) {
	if (other.getEntityType() != PlayerEntity::ID) return;
	if (const auto player = dynamic_cast<PlayerEntity*>(&other); this->source != player->getId() && this->getDistanceTo(other) < 8) {
		if (player != nullptr) {
			if (player->damage(this->source, 1)) {
				this->hasHit = true;
			}
		}
	}
}

void BulletEntity::onWallCollision() {
	this->hasHit = true;
}

bool BulletEntity::isMarkedForRemoval() const {
	return this->hasHit;
}

void BulletEntity::writeToPacket(sf::Packet& packet) const {
	Entity::writeToPacket(packet);
	packet << this->source;
	packet << this->colour.r;
	packet << this->colour.g;
	packet << this->colour.b;
	packet << this->getVelocity().x;
	packet << this->getVelocity().y;
}

void BulletEntity::readFromPacket(sf::Packet& packet) {
	Entity::readFromPacket(packet);
	packet >> this->source;
	uint8_t r, g, b;
	packet >> r >> g >> b;
	const sf::Color colour(r, g, b, 0xFF);
	this->colour = colour;
	float xVel, yVel;
	packet >> xVel;
	packet >> yVel;
	this->setVelocity({xVel, yVel});
}