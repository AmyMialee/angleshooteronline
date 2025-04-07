#pragma once

class PlayerEntity : public Entity {
	bool isMarkedForRemoval() const override;

public:
	std::string name;
	sf::Vector2f input = {0, 0};
	bool isFiring = false;
	sf::Color colour = {0xFF, 0xAA, 0xAA, 0xFF};
	int health = 8;
	int deathTime = 0;
	int immunityTime = 0;
	int bulletCharge = 0;
	int score = 0;

	explicit PlayerEntity(uint16_t id, World* world);
	inline static auto ID = Identifier("player");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	virtual bool damage(uint16_t source, int amount);
	virtual void onDeath(uint16_t source);

	void writeToPacket(sf::Packet& packet) const override;
	void readFromPacket(sf::Packet& packet) override;
};