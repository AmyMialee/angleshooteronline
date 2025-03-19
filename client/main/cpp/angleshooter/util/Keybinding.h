#pragma once

class Keybinding {
	std::shared_ptr<Identifier> id;
	sf::Keyboard::Scancode defaultKey;
	sf::Keyboard::Scancode key;
	bool pressed;
	int timesPressed;
	
public:
	explicit Keybinding(std::shared_ptr<Identifier> id, sf::Keyboard::Scancode defaultKey);
	[[nodiscard]] Identifier* getId() const;
	[[nodiscard]] sf::Keyboard::Scancode getDefaultKey() const;
	[[nodiscard]] sf::Keyboard::Scancode getKey() const;
	[[nodiscard]] bool isPressed() const;
	[[nodiscard]] bool wasPressed();
	void onPressed();
	void onReleased();
	void loadBinding(sf::Keyboard::Scancode scan);
	void rebind(sf::Keyboard::Scancode scan);
};
