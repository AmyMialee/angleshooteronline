#pragma once

class InputManager final {
protected:
	explicit InputManager();
	~InputManager() = default;

public:
	void handleInput(sf::RenderWindow& window);

	InputManager(const InputManager&) = delete;
	void operator=(const InputManager&) = delete;
	static InputManager& get() {
		static InputManager instance;
		return instance;
	}
};