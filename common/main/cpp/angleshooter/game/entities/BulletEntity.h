#pragma once

class BulletEntity final : public Entity {
	int colour = 0xFFFFFFFF;
	bool hasHit = false;
	[[nodiscard]] bool isMarkedForRemoval() const override;

public:
	explicit BulletEntity(World* world, int colour, sf::Vector2f position, sf::Vector2f velocity);
	inline static auto ID = Identifier("bullet");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	void onCollision(Entity& other) override;
	void onWallCollision() override;
};
