#pragma once

class ClientPlayerEntity final : public PlayerEntity {
	bool syncedFiring = false;
	sf::Vector2f syncedInput = sf::Vector2f(0, 0);
	sf::Vector2f syncedPosition = sf::Vector2f(0, 0);
public:
	bool isClientPlayer = false;
	explicit ClientPlayerEntity(uint16_t id, World* world);
	void tick(float deltaTime) override;
	void readFromPacket(sf::Packet& packet) override;
};