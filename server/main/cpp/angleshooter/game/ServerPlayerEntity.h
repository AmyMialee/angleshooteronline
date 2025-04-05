#pragma once

class ServerPlayerEntity final : public PlayerEntity {
public:
	bool shouldBeErased = false;
	ServerPlayerEntity(uint16_t id, World* world);
	void tick(float deltaTime) override;
	void onDeath(sf::Color sourceColour) override;

private:
	[[nodiscard]] bool isMarkedForRemoval() const override;
};
