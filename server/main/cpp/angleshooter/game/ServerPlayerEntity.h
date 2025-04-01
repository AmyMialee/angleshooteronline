#pragma once

class ServerPlayerEntity final : public PlayerEntity {
public:
	ServerPlayerEntity(World* world, const std::string& name);
	void tick(float deltaTime) override;
};
