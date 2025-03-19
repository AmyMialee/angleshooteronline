#pragma once

class BulletEntity final : public Entity {
	PlayerEntity& owner;
	bool hasHit = false;
	[[nodiscard]] bool isMarkedForRemoval() const override;

protected:
	void render(float deltaTime) override;

public:
	explicit BulletEntity(PlayerEntity& owner);
	void tick(float deltaTime) override;
	void onCollision(GameObject& other) override;
	void onWallCollision() override;
};
