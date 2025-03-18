#pragma once

class InputManager {
	std::vector<std::shared_ptr<Keybinding>> keybindings;
	Keybinding* selectedKeybinding = nullptr;
	std::shared_ptr<Keybinding> up;
	std::shared_ptr<Keybinding> down;
	std::shared_ptr<Keybinding> left;
	std::shared_ptr<Keybinding> right;
	std::shared_ptr<Keybinding> fire;
	
public:
	InputManager();
	void handleInput(sf::RenderWindow& window, EventHandler& states);
	void onKeyPressed(sf::Keyboard::Scancode code);
	void onKeyReleased(sf::Keyboard::Scancode code);
	Keybinding* getSelectedKeybinding();
	void setSelectedKeybinding(Keybinding* key);
	void rebindKey(sf::Keyboard::Scan key, const std::function<void()>& run);
	bool isKeyTaken(sf::Keyboard::Scan key);
	[[nodiscard]] std::vector<std::shared_ptr<Keybinding>>& getKeybindings();
	Keybinding* getUp();
	Keybinding* getDown();
	Keybinding* getLeft();
	Keybinding* getRight();
	Keybinding* getFire();
};