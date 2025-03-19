#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "GameObject.h"

GameObject::GameObject() : id(0) {
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineColor(sf::Color::Green);
	hitbox.setOutlineThickness(1.f);
}

void GameObject::baseTick(float deltaTime) {
	tick(deltaTime);
}

void GameObject::baseRender(float deltaTime) {
	render(deltaTime);
	if (OptionsManager::get().areHitboxesEnabled()) this->renderHitbox(deltaTime);
}

void GameObject::renderHitbox(float deltaTime) {
	const auto rect = this->getBoundingBox();
	if (hitbox.getPosition() != rect.position) hitbox.setPosition(rect.position);
	if (hitbox.getSize() != rect.size) hitbox.setSize(rect.size);
	ClientContext::get()->getRenderTexture()->draw(hitbox);
	const auto center = this->getPosition();
	const auto rotation = this->getRotation().asRadians();
	const auto endPoint = center + sf::Vector2f(std::cos(rotation) * 16, std::sin(rotation) * 16);
	sf::VertexArray line(sf::PrimitiveType::Lines, 2);
	line[0].position = center;
	line[0].color = sf::Color::Red;
	line[1].position = endPoint;
	line[1].color = sf::Color::Red;
	ClientContext::get()->getRenderTexture()->draw(line);
}

float GameObject::getX() const {
	return this->getPosition().x;
}

float GameObject::getY() const {
	return this->getPosition().y;
}

uint32_t GameObject::getId() const {
	return this->id;
}

void GameObject::setId(uint32_t id) {
	this->id = id;
}

sf::FloatRect GameObject::getBoundingBox() const {
	return {this->getPosition() - this->getScale().componentWiseDiv({2, 2}), this->getScale()};
}

bool GameObject::isColliding(const GameObject& other) const {
	return getBoundingBox().findIntersection(other.getBoundingBox()).has_value();
}

float GameObject::getDistanceTo(const GameObject& other) const {
	const auto center = this->getPosition();
	const auto otherCenter = other.getPosition();
	return static_cast<float>(std::sqrt(std::pow(center.x - otherCenter.x, 2) + std::pow(center.y - otherCenter.y, 2)));
}

bool GameObject::isInWall(sf::Vector2f pos) const {
	const auto left = static_cast<uint16_t>((pos.x - this->getScale().x / 2) / 16);
	const auto top = static_cast<uint16_t>((pos.y - this->getScale().y / 2) / 16);
	const auto right = static_cast<uint16_t>((pos.x + this->getScale().x / 2) / 16);
	const auto bottom = static_cast<uint16_t>((pos.y + this->getScale().y / 2) / 16);
	for (auto x = left; x <= right; x++) {
		for (auto y = top; y <= bottom; y++) {
			if (ClientContext::get()->getWorld()->getMap().isSolid(y, x)) return true;
		}
	}
	return false;
}

void GameObject::tick(float deltaTime) {}

void GameObject::render(float deltaTime) {}

void GameObject::onCollision(GameObject& other) {}

bool GameObject::isMarkedForRemoval() const {
	return false;
}

bool GameObject::operator==(const GameObject& other) const {
	return this == &other;
}

bool GameObject::operator!=(const GameObject& other) const {
	return !(*this == other);
}