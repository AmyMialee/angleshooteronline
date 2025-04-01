#include "PreCompiledClient.h"
#include "JoiningState.h"

const Identifier JoiningState::JOINING_ID("joining");

void JoiningState::init() {}

void JoiningState::loadAssets() {}

void JoiningState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MenuState::MENU_TEXTURE));
	static sf::Text ipAddressText(FontHolder::getInstance().getDefault());
	static std::once_flag flag;
	std::call_once(flag, [&] {
		Util::centre(background);
		background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		background.setColor({165, 255, 255, 255});
		background.setScale({2.f, 2.f});
		Util::centre(ipAddressText);
		ipAddressText.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		ipAddressText.setString("Enter IP Address: " + ipAddress);
	});
	auto& texture = AngleShooterClient::get().renderTexture;
	texture.draw(background);
	ipAddressText.setString("Enter IP Address: " + ipAddress);
	texture.draw(ipAddressText);
}

bool JoiningState::shouldRenderNextState() const {
	return false;
}

bool JoiningState::tick(float deltaTime) {
	return false;
}

/**
 * IP Address Regex taken from https://www.oreilly.com/library/view/regular-expressions-cookbook/9780596802837/ch07s16.html
 */
bool JoiningState::handleEvent(const sf::Event& event) {
	if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
		if ((text->unicode >= '0' && text->unicode <= '9') || text->unicode == '.') {
			if (ipAddress.size() < 15) ipAddress += static_cast<char>(text->unicode);
		}
		return true;
	}
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			requestStackPop();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace) {
			if (!ipAddress.empty()) ipAddress.pop_back();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
			static std::regex ipPattern("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
			if (std::regex_match(ipAddress, ipPattern)) {
				OptionsManager::get().setIp(ipAddress);
				requestStackClear();
				requestStackPush(MenuState::MENU_ID);
			}
		}
		return true;
	}
	return false;
}

Identifier JoiningState::getId() {
	return JOINING_ID;
}

Identifier JoiningState::getStateId() {
	return JOINING_ID;
}