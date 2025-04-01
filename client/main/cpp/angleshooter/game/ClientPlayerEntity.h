#pragma once

class ClientPlayerEntity : public PlayerEntity {
	bool isClientPlayer;

public:
	ClientPlayerEntity(World* world, const std::string& name, bool isClientPlayer);
	void tick(float deltaTime) override;
};