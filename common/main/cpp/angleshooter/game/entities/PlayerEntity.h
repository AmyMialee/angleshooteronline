﻿#pragma once

class PlayerEntity : public Entity {
	std::string name;
	int health = 8;
	int deathTime = 0;
	int immunityTime = 0;
	bool isMarkedForRemoval() const override;

public:
	sf::Vector2f input = {0, 0};
	bool isFiring = false;
	int color = 0;
	int bulletCharge = 0;

	explicit PlayerEntity(World* world, std::string name);
	inline static auto ID = Identifier("player");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	bool damage(int sourceColour, int amount);
	void onDeath(int sourceColour);
	[[nodiscard]] std::string getName() const;
	[[nodiscard]] int getColour() const;
	[[nodiscard]] int getHealth() const;
	[[nodiscard]] int getDeathTime() const;
	[[nodiscard]] int getImmunityTime() const;
	[[nodiscard]] int getBulletCharge() const;
	void setColor(int color);
};