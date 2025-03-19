#include "PreCompiledClient.h"
#include "Keybinding.h"

Keybinding::Keybinding(std::shared_ptr<Identifier> id, sf::Keyboard::Scancode defaultKey) : id(std::shared_ptr(id)), defaultKey(defaultKey), key(defaultKey), pressed(false), timesPressed(0) {}

Identifier* Keybinding::getId() const {
	return this->id.get();
}

sf::Keyboard::Scancode Keybinding::getDefaultKey() const {
	return this->defaultKey;
}

sf::Keyboard::Scancode Keybinding::getKey() const {
	return this->key;
}

bool Keybinding::isPressed() const {
	return this->pressed;
}

bool Keybinding::wasPressed() {
	if (timesPressed <= 0) return false;
	timesPressed--;
	return true;
}

void Keybinding::onPressed() {
	this->timesPressed++;
	this->pressed = true;
}

void Keybinding::onReleased() {
	this->pressed = false;
}

void Keybinding::loadBinding(sf::Keyboard::Scancode scan) {
	this->key = scan;
}

void Keybinding::rebind(sf::Keyboard::Scancode scan) {
	this->key = scan;
	OptionsManager::get().saveToFile();
}
