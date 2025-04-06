#include "PreCompiledClient.h"
#include "IpButton.h"

IpButton::IpButton() {
	this->Button::setToggle(true);
	setCallback([this]{});
}

void IpButton::setTextFunction(const std::function<std::string(std::string)>& textFunction) {
	this->textFunction = textFunction;
}

/**
 * IP Address Regex taken from https://www.oreilly.com/library/view/regular-expressions-cookbook/9780596802837/ch07s16.html
 */
void IpButton::handleEvent(const sf::Event& event) {
	if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
		if (((text->unicode >= '0' && text->unicode <= '9') || text->unicode == '.') && ipAddress.size() < 15) {
			ipAddress += static_cast<char>(text->unicode);
			if (this->textFunction) this->setText(this->textFunction(ipAddress));
		}
		return;
	}
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			this->ipAddress = OptionsManager::get().getIp();
			setPressed(false);
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace) {
			if (!ipAddress.empty()) ipAddress.pop_back();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
			static std::regex ipPattern("^(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$");
			if (std::regex_match(ipAddress, ipPattern)) {
				OptionsManager::get().setIp(ipAddress);
				setPressed(false);
			}
		}
		if (this->textFunction) this->setText(this->textFunction(ipAddress));
		return;
	}
	Button::handleEvent(event);
}