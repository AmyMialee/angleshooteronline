#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "BulletEntity.h"

BulletEntity::BulletEntity(World* world, PlayerEntity& owner) : Entity(world), owner(owner) {
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
	if (this->owner != other && this->getDistanceTo(other) < 8) {
		if (const auto player = dynamic_cast<PlayerEntity*>(&other); player != nullptr) {
			if (player->damage(&owner, 1)) {
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
