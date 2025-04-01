#include "PreCompiledClient.h"
#include "ClientPlayerEntity.h"

ClientPlayerEntity::ClientPlayerEntity(World* world, const std::string& name, bool isClientPlayer) : PlayerEntity(world, name), isClientPlayer(isClientPlayer) {}

void ClientPlayerEntity::tick(float deltaTime) {
	const auto lastFiring = isFiring;
	isFiring = false;
	PlayerEntity::tick(deltaTime);
	isFiring = lastFiring;
	if (!isClientPlayer) return;
	const auto lastInput = input;
	input = sf::Vector2f(0, 0);
	if (InputManager::get().getUp()->isPressed()) input += sf::Vector2f(0, -1);
	if (InputManager::get().getDown()->isPressed()) input += sf::Vector2f(0, 1);
	if (InputManager::get().getLeft()->isPressed()) input += sf::Vector2f(-1, 0);
	if (InputManager::get().getRight()->isPressed()) input += sf::Vector2f(1, 0);
	isFiring = InputManager::get().getFire()->isPressed();
	if (this->world->getAge() % 12 == 0 && (lastInput != input || lastFiring != isFiring)) {
		auto packet = NetworkProtocol::C2S_PLAYER_INPUT.getPacket();
		packet << input.x << input.y << isFiring;
		AngleShooterClient::get().send(packet);
	}
}