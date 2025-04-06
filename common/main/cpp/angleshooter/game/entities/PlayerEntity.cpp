#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(uint16_t id, World* world) : Entity(id, world) {
	this->setDrag(0.35f);
	this->setScale({14, 14});
}

const Identifier& PlayerEntity::getEntityType() const {
	return ID;
}

void PlayerEntity::tick(float deltaTime) {
	if (this->deathTime > 0) {
		this->deathTime--;
		return;
	}
	if (this->immunityTime > 0) {
		this->immunityTime--;
		this->bulletCharge = 0;
	} else {
		this->bulletCharge++;
		this->bulletCharge = std::min(this->bulletCharge, 120);
	}
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
	Entity::tick(deltaTime);
}

bool PlayerEntity::damage(uint16_t source, int amount) {
	if (this->health <= 0 || this->immunityTime > 0) return false;
	this->health -= amount;
	if (this->health <= 0) {
		this->health = 0;
		this->onDeath(source);
	}
	return true;
}

void PlayerEntity::onDeath(uint16_t source) {
	this->deathTime = 60;
	this->immunityTime = 120;
}

std::string PlayerEntity::getName() const {
	return this->name;
}

sf::Color PlayerEntity::getColour() const {
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

void PlayerEntity::setColor(sf::Color color) {
	this->color = color;
}

bool PlayerEntity::isMarkedForRemoval() const {
	return false;
}

void PlayerEntity::writeToPacket(sf::Packet& packet) const {
	Entity::writeToPacket(packet);
	packet << this->name;
	packet << this->color.r << this->color.g << this->color.b;
	packet << this->health;
	packet << this->input.x;
	packet << this->input.y;
	packet << this->getVelocity().x;
	packet << this->getVelocity().y;
}

void PlayerEntity::readFromPacket(sf::Packet& packet) {
	Entity::readFromPacket(packet);
	packet >> this->name;
	packet >> this->color.r;
	packet >> this->color.g;
	packet >> this->color.b;
	packet >> this->health;
	packet >> this->input.x;
	packet >> this->input.y;
	float vx, vy;
	packet >> vx;
	packet >> vy;
	this->setVelocity({vx, vy});
}