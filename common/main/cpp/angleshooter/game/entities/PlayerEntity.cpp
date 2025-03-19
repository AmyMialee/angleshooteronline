#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(std::string id) : id(std::move(id)) {
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
			this->setPosition(ClientContext::get()->getWorld()->getMap().getRandomSpawnpoint());
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
	auto input = sf::Vector2f();
	static Identifier shootSound("bullet.ogg");
	if (ClientContext::get()->getInputManager()->getUp()->isPressed()) input = input + sf::Vector2f(0, -1);
	if (ClientContext::get()->getInputManager()->getDown()->isPressed()) input = input + sf::Vector2f(0, 1);
	if (ClientContext::get()->getInputManager()->getLeft()->isPressed()) input = input + sf::Vector2f(-1, 0);
	if (ClientContext::get()->getInputManager()->getRight()->isPressed()) input = input + sf::Vector2f(1, 0);
	if (ClientContext::get()->getInputManager()->getFire()->isPressed() && this->bulletCharge >= 12) {
		this->bulletCharge -= 12;
		const auto world = ClientContext::get()->getWorld();
		const auto bullet = std::make_shared<BulletEntity>(*this);
		bullet->setPosition(this->getPosition());
		bullet->setRotation(this->getRotation());
		auto x = std::cos(this->getRotation().asRadians());
		auto y = std::sin(this->getRotation().asRadians());
		x += Util::randomNormalFloat(0.025f);
		y += Util::randomNormalFloat(0.025f);
		const auto velocity = sf::Vector2f(x, y);
		bullet->setVelocity(velocity * 8.f);
		world->addGameObject(bullet);
		ClientContext::get()->getAudioManager()->playSound(shootSound, .6f, Util::randomFloat(1.f, 1.6f));
	}
	if (input.length() > 0) {  // NOLINT(clang-diagnostic-undefined-func-template)
		input = input.componentWiseDiv({input.length(), input.length()});
		constexpr auto movementSpeed = 1.6f;
		input = input.componentWiseMul({movementSpeed, movementSpeed});
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

bool PlayerEntity::damage(PlayerEntity* source, int amount) {
	if (this->health <= 0 || this->immunityTime) return false;
	this->health -= amount;
	if (this->health <= 0) {
		this->health = 0;
		this->onDeath(source);
	} else {
		static Identifier hurtSound("hurt.ogg");
		AudioManager::get().playSound(hurtSound, .8f, Util::randomFloat(0.8f, 1.2f));
	}
	return true;
}

void PlayerEntity::onDeath(PlayerEntity* source) {
	this->deathTime = 60;
	this->immunityTime = 120;
	if (source != nullptr) {
		for (auto i = 0; i < 20; i++) {
			const auto world = ClientContext::get()->getWorld();
			const auto bullet = std::make_shared<BulletEntity>(*source);
			bullet->setPosition(this->getPosition());
			const auto x = static_cast<float>(std::sin((18 * i + 25) * (std::numbers::pi / 180)));
			const auto y = static_cast<float>(std::cos((18 * i + 25) * (std::numbers::pi / 180)));
			const auto velocity = sf::Vector2f(x, y);
			bullet->setVelocity(velocity * 1.28f);
			bullet->setRotation(sf::radians(std::atan2(velocity.y, velocity.x)));
			world->addGameObject(bullet);
		}
	}
	static Identifier explodeSound("explode.ogg");
	AudioManager::get().playSound(explodeSound, .8f, Util::randomFloat(1.2f, 1.8f));
	if (const auto data = ClientContext::get()->getWorld()->getPlayerData(this->id); data != nullptr) {
		data->addScore(1);
	}
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

bool PlayerEntity::isMarkedForRemoval() const {
	return false;
}
