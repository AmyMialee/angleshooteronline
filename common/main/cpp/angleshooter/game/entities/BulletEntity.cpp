#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "BulletEntity.h"

BulletEntity::BulletEntity(PlayerEntity& owner) : Entity(), owner(owner) {
	this->setDrag(0.f);
	this->setScale({6, 6});
}

void BulletEntity::render(float deltaTime) {
	static sf::Sprite bullet1(ClientContext::get()->getTextureHolder()->get(Identifier("bullet.png")));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		Util::centre(bullet1);
		const auto textureSize1 = bullet1.getTexture().getSize();
		bullet1.setScale(this->getScale().componentWiseDiv({static_cast<float>(textureSize1.x), static_cast<float>(textureSize1.y)}));
	});
	const auto pos = this->getPosition() + this->getVelocity() * deltaTime;
	auto sprite = bullet1;
	sprite.setPosition(pos);
	sprite.setRotation(this->getRotation());
	ClientContext::get()->getRenderTexture()->draw(sprite);
}

void BulletEntity::tick(float deltaTime) {
	Entity::tick(deltaTime);
}

void BulletEntity::onCollision(GameObject& other) {
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
