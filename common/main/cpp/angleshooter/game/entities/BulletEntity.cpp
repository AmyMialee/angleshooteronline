#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "BulletEntity.h"

BulletEntity::BulletEntity(World* world, int colour, sf::Vector2f position, sf::Vector2f velocity) : Entity(world), colour(colour) {
	this->setPosition(position);
	this->setVelocity(velocity);
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
	if (const auto player = dynamic_cast<PlayerEntity*>(&other); this->colour != player->getColour() && this->getDistanceTo(other) < 8) {
		if (player != nullptr) {
			if (player->damage(this->colour, 1)) {
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
