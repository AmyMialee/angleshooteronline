#pragma once

class BulletEntity final : public Entity {
	PlayerEntity& owner;
	bool hasHit = false;
	[[nodiscard]] bool isMarkedForRemoval() const override;

public:
	explicit BulletEntity(World* world, uint16_t id, PlayerEntity& owner);
	inline static auto ID = Identifier("bullet");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	void onCollision(Entity& other) override;
	void onWallCollision() override;
};
