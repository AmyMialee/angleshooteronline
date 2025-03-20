#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Entity.h"

Entity::Entity(World* world) : id(world->getNextId()), world(world) {}

float Entity::getX() const {
	return this->getPosition().x;
}

float Entity::getY() const {
	return this->getPosition().y;
}

uint32_t Entity::getId() const {
	return this->id;
}

sf::FloatRect Entity::getBoundingBox() const {
	return {this->getPosition() - this->getScale().componentWiseDiv({2, 2}), this->getScale()};
}

bool Entity::isColliding(const Entity& other) const {
	return getBoundingBox().findIntersection(other.getBoundingBox()).has_value();
}

float Entity::getDistanceTo(const Entity& other) const {
	const auto center = this->getPosition();
	const auto otherCenter = other.getPosition();
	return static_cast<float>(std::sqrt(std::pow(center.x - otherCenter.x, 2) + std::pow(center.y - otherCenter.y, 2)));
}

bool Entity::isInWall(sf::Vector2f pos) const {
	const auto left = static_cast<uint16_t>((pos.x - this->getScale().x / 2) / 16);
	const auto top = static_cast<uint16_t>((pos.y - this->getScale().y / 2) / 16);
	const auto right = static_cast<uint16_t>((pos.x + this->getScale().x / 2) / 16);
	const auto bottom = static_cast<uint16_t>((pos.y + this->getScale().y / 2) / 16);
	for (auto x = left; x <= right; x++) {
		for (auto y = top; y <= bottom; y++) {
			if (this->world->getMap().isSolid(y, x)) return true;
		}
	}
	return false;
}

void Entity::tick(float deltaTime) {
	this->addPosition(this->getVelocity());
	auto multiplier = 1.f - this->getDrag();
	this->setVelocity(this->getVelocity().componentWiseMul({multiplier, multiplier}));
}

void Entity::onCollision(Entity& other) {}

bool Entity::operator==(const Entity& other) const {
	return this == &other;
}

bool Entity::operator!=(const Entity& other) const {
	return !(*this == other);
}

void Entity::onWallCollision() {}

sf::Vector2f Entity::getVelocity() const {
	return this->velocity;
}

void Entity::addVelocity(sf::Vector2f velocity) {
	this->setVelocity(this->getVelocity() + velocity);
}

void Entity::setVelocity(sf::Vector2f velocity) {
	this->velocity = velocity;
}

void Entity::addPosition(sf::Vector2f change) {
	auto x = this->getX();
	auto y = this->getY();
	auto hit = false;
	while (true) {
		if (std::abs(change.x) < 0.1) break;
		if (!this->isInWall({x + change.x, y})) {
			x += change.x;
			break;
		}
		change = {change.x / 2, change.y};
		this->setVelocity(change);
		hit = true;
	}
	while (true) {
		if (std::abs(change.y) < 0.1) break;
		if (!this->isInWall({x, y + change.y})) {
			y += change.y;
			break;
		}
		change = {change.x, change.y / 2};
		this->setVelocity(change);
		hit = true;
	}
	this->setPosition({x, y});
	if (hit) this->onWallCollision();
}

float Entity::getDrag() const {
	return this->drag;
}

void Entity::setDrag(float drag) {
	this->drag = drag;
}