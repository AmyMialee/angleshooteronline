#pragma once

class PlayerEntity : public Entity {
	bool isMarkedForRemoval() const override;

public:
	std::string name;
	sf::Vector2f input = {0, 0};
	bool isFiring = false;
	sf::Color colour = {0xFF, 0xAA, 0xAA, 0xFF};
	uint16_t health = 8;
	uint16_t deathTime = 0;
	uint16_t immunityTime = 0;
	uint16_t bulletCharge = 0;
	uint16_t score = 0;

	explicit PlayerEntity(uint16_t id, World* world);
	inline static auto ID = Identifier("player");
	const Identifier& getEntityType() const override;
	void tick(float deltaTime) override;
	virtual bool damage(uint16_t source, uint16_t amount);
	virtual void onDeath(uint16_t source);

	void writeToPacket(sf::Packet& packet) const override;
	void readFromPacket(sf::Packet& packet) override;
};