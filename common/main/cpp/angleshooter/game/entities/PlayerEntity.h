#pragma once

class PlayerEntity final : public Entity {
	std::string id;
	int health = 8;
	int deathTime = 0;
	int immunityTime = 0;
	int bulletCharge = 0;
	bool isMarkedForRemoval() const override;
	
protected:
	void render(float deltaTime) override;

public:
	explicit PlayerEntity(std::string id);
	void tick(float deltaTime) override;
	bool damage(PlayerEntity* source, int amount);
	void onDeath(PlayerEntity* source);
	[[nodiscard]] int getHealth() const;
	[[nodiscard]] int getDeathTime() const;
	[[nodiscard]] int getImmunityTime() const;
	[[nodiscard]] int getBulletCharge() const;
	[[nodiscard]] std::string getId() const;
};
