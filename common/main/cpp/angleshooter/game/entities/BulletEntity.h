#pragma once

class BulletEntity final : public Entity {
	bool hasHit = false;
	[[nodiscard]] bool isMarkedForRemoval() const override;

public:
	sf::Color colour = {0xFF, 0xAA, 0xAA, 0xFF};
	explicit BulletEntity(uint16_t id, World* world);
	inline static auto ID = Identifier("bullet");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	void onCollision(Entity& other) override;
	void onWallCollision() override;

	void writeToPacket(sf::Packet& packet) const override;
	void readFromPacket(sf::Packet& packet) override;
};
