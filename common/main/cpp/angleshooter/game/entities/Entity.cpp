#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Entity.h"

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

void Entity::tick(float deltaTime) {
	this->addPosition(this->getVelocity());
	auto multiplier = 1.f - this->getDrag();
	this->setVelocity(this->getVelocity().componentWiseMul({multiplier, multiplier}));
}

float Entity::getDrag() const {
	return this->drag;
}

void Entity::setDrag(float drag) {
	this->drag = drag;
}

void Entity::renderHitbox(float deltaTime) {
	const auto rect = this->getBoundingBox();
	sf::RectangleShape shape;
	shape.setPosition(rect.position + this->getVelocity() * deltaTime);
	shape.setSize(rect.size);
	shape.setFillColor(sf::Color::Transparent);
	shape.setOutlineColor(sf::Color::Green);
	shape.setOutlineThickness(1.f);
	ClientContext::get()->getRenderTexture()->draw(shape);
	const auto center = this->getPosition() + this->getVelocity() * deltaTime;
	const auto rotation = this->getRotation().asRadians();
	const auto endPoint = center + sf::Vector2f(std::cos(rotation) * 16, std::sin(rotation) * 16);
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].position = center;
	line[0].color = sf::Color::Red;
	line[1].position = endPoint;
	line[1].color = sf::Color::Red;
	ClientContext::get()->getRenderTexture()->draw(line);
}
