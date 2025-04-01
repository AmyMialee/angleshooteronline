#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(World* world, std::string name) : Entity(world), name(std::move(name)) {
	this->setDrag(0.35f);
	this->setScale({14, 14});
}

const Identifier& PlayerEntity::getEntityType() const {
	return ID;
}

void PlayerEntity::tick(float deltaTime) {
	if (this->deathTime > 0) {
		this->deathTime--;
		if (this->deathTime <= 0) {
			this->health = 8;
			this->setPosition(this->world->getMap()->getRandomSpawnpoint());
		}
		return;
	}
	if (this->immunityTime > 0) {
		this->immunityTime--;
		this->bulletCharge = 0;
	} else {
		this->bulletCharge++;
		this->bulletCharge = std::min(this->bulletCharge, 120);
	}
	Entity::tick(deltaTime);
	if (input.length() > 0) {  // NOLINT(clang-diagnostic-undefined-func-template)
		input /= input.length();
		constexpr auto movementSpeed = 1.6f;
		input *= movementSpeed;
		this->addVelocity(input);
		const auto currentRotation = this->getRotation();
		const auto targetRotation = sf::radians(std::atan2(input.y, input.x));
		auto rotationDifference = targetRotation - currentRotation;
		if (rotationDifference.asRadians() > std::numbers::pi) {
			rotationDifference -= sf::radians(2 * static_cast<float>(std::numbers::pi));
		} else if (rotationDifference.asRadians() < -std::numbers::pi) {
			rotationDifference += sf::radians(2 * static_cast<float>(std::numbers::pi));
		}
		const auto newRotation = currentRotation + rotationDifference * 0.75f;
		this->setRotation(newRotation);
	}
}

bool PlayerEntity::damage(int sourceColour, int amount) {
	if (this->health <= 0 || this->immunityTime) return false;
	this->health -= amount;
	if (this->health <= 0) {
		this->health = 0;
		this->onDeath(sourceColour);
	} else {
		static Identifier hurtSound("hurt.ogg");
		this->world->playSound(hurtSound, .8f, Util::randomFloat(0.8f, 1.2f));
	}
	return true;
}

void PlayerEntity::onDeath(int sourceColour) {
	this->deathTime = 60;
	this->immunityTime = 120;
	for (auto i = 0; i < 20; i++) {
		const auto x = static_cast<float>(std::sin((18 * i + 25) * (std::numbers::pi / 180)));
		const auto y = static_cast<float>(std::cos((18 * i + 25) * (std::numbers::pi / 180)));
		const auto velocity = sf::Vector2f(x, y);
		const auto bullet = this->world->spawnEntity(std::make_shared<BulletEntity>(this->world, sourceColour, this->getPosition(), velocity * 1.28f));
		bullet->setRotation(sf::radians(std::atan2(velocity.y, velocity.x)));
	}
	static Identifier explodeSound("explode.ogg");
	this->world->playSound(explodeSound, .8f, Util::randomFloat(1.2f, 1.8f));
}

std::string PlayerEntity::getName() const {
	return this->name;
}

int PlayerEntity::getColour() const {
	return this->color;
}

int PlayerEntity::getHealth() const {
	return this->health;
}

int PlayerEntity::getDeathTime() const {
	return this->deathTime;
}

int PlayerEntity::getImmunityTime() const {
	return this->immunityTime;
}

int PlayerEntity::getBulletCharge() const {
	return this->bulletCharge;
}

void PlayerEntity::setColor(int color) {
	this->color = color;
}

bool PlayerEntity::isMarkedForRemoval() const {
	return false;
}
