#pragma once

class PlayerEntity : public Entity {
	std::string name;
	int health = 8;
	int deathTime = 0;
	int immunityTime = 0;
	int bulletCharge = 0;
	bool isMarkedForRemoval() const override;
	
public:
	explicit PlayerEntity(World* world, uint16_t id, std::string name);
	inline static auto ID = Identifier("player");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	bool damage(PlayerEntity* source, int amount);
	void onDeath(PlayerEntity* source);
	[[nodiscard]] int getHealth() const;
	[[nodiscard]] int getDeathTime() const;
	[[nodiscard]] int getImmunityTime() const;
	[[nodiscard]] int getBulletCharge() const;
};