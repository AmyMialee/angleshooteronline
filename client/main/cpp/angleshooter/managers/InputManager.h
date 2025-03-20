#pragma once

class InputManager final : public Singleton<InputManager> {
	friend class Singleton;
	std::vector<std::shared_ptr<Keybinding>> keybindings;
	Keybinding* selectedKeybinding = nullptr;
	std::shared_ptr<Keybinding> up;
	std::shared_ptr<Keybinding> down;
	std::shared_ptr<Keybinding> left;
	std::shared_ptr<Keybinding> right;
	std::shared_ptr<Keybinding> fire;
	InputManager();

public:
	void handleInput(sf::RenderWindow& window);
	void onKeyPressed(sf::Keyboard::Scancode code);
	void onKeyReleased(sf::Keyboard::Scancode code);
	Keybinding* getSelectedKeybinding();
	void setSelectedKeybinding(Keybinding* key);
	bool isKeyTaken(sf::Keyboard::Scan key);
	[[nodiscard]] std::vector<std::shared_ptr<Keybinding>>& getKeybindings();
	Keybinding* getUp();
	Keybinding* getDown();
	Keybinding* getLeft();
	Keybinding* getRight();
	Keybinding* getFire();
};