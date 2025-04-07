#pragma once

class ServerPlayerEntity final : public PlayerEntity {
public:
	bool shouldBeErased = false;

	ServerPlayerEntity(uint16_t id, World* world);
	void tick(float deltaTime) override;
	bool damage(uint16_t source, uint16_t amount) override;
	void onDeath(uint16_t source) override;

private:
	[[nodiscard]] bool isMarkedForRemoval() const override;
};
