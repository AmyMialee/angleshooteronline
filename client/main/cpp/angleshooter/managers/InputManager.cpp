#include "PreCompiledClient.h"
#include "InputManager.h"

#include "../AngleShooterClient.h"

InputManager::InputManager() {}

void InputManager::handleInput(sf::RenderWindow& window) {
	while (const auto optional = window.pollEvent()) {
		if (!optional.has_value()) continue;
		auto event = optional.value();
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			auto packet = NetworkProtocol::CHAT_MESSAGE->getPacket(AngleShooterClient::get().server);
			packet << static_cast<std::string>("Key pressed: " + getDescription(keyPressed->scancode));
			AngleShooterClient::get().send(packet);
		} else if (const auto* keyReleased = event.getIf<sf::Event::KeyReleased>()) {

		} else if (event.is<sf::Event::Closed>()) {
			window.close();
			return;
		}
	}
}