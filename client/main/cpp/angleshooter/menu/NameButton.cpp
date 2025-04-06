#include "PreCompiledClient.h"
#include "NameButton.h"

NameButton::NameButton() {
	this->Button::setToggle(true);
	setCallback([this]{});
}

void NameButton::setTextFunction(const std::function<std::string(std::string)>& textFunction) {
	this->textFunction = textFunction;
}

void NameButton::handleEvent(const sf::Event& event) {
	if (const auto* text = event.getIf<sf::Event::TextEntered>()) {
		if (text->unicode >= '!' && text->unicode <= '~' && name.size() < 16) {
			name += static_cast<char>(text->unicode);
			if (this->textFunction) this->setText(this->textFunction(name));
		}
		return;
	}
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			this->name = OptionsManager::get().getName();
			setPressed(false);
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Backspace) {
			if (!name.empty()) name.pop_back();
		} else if (keyPressed->scancode == sf::Keyboard::Scan::Enter && name.size() >= 1) {
			OptionsManager::get().setName(name);
			setPressed(false);
		}
		if (this->textFunction) this->setText(this->textFunction(name));
		return;
	}
	Button::handleEvent(event);
}