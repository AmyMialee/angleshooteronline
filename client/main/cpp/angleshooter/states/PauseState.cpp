#include "PreCompiledClient.h"
#include "PauseState.h"

const Identifier PauseState::PAUSE_ID("pause");

void PauseState::init() {
	const auto playButton = std::make_shared<Button>();
	playButton->setPosition({250, 250});
	playButton->setText("Resume");
	playButton->setCallback([this] {
		requestStackPop();
	});
	const auto settingsButton = std::make_shared<Button>();
	settingsButton->setPosition({250, 300});
	settingsButton->setText("Settings");
	settingsButton->setCallback([this] {
		requestStackPush(SettingsState::getId());
	});
	const auto exitButton = std::make_shared<Button>();
	exitButton->setPosition({250, 350});
	exitButton->setText("Exit");
	exitButton->setCallback([this] {
		requestStackClear();
		requestStackPush(MenuState::getId());
	});
	gui.pack(playButton);
	gui.pack(settingsButton);
	gui.pack(exitButton);
}

void PauseState::loadAssets() {}

void PauseState::render(float deltaTime) {
	static sf::RectangleShape backgroundShape;
	static std::once_flag flag;
	std::call_once(flag, [&] {
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
		backgroundShape.setSize(AngleShooterClient::get().window.getView().getSize());
	});
	auto& texture = AngleShooterClient::get().renderTexture;
	texture.draw(backgroundShape);
	texture.setView(texture.getDefaultView());
	texture.draw(gui);
}

bool PauseState::shouldRenderNextState() const {
	return true;
}

bool PauseState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return true;
}

bool PauseState::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			requestStackPop();
			return false;
		}
	}
	gui.handleEvent(event);
	return false;
}

Identifier PauseState::getId() {
	return PAUSE_ID;
}

Identifier PauseState::getStateId() {
	return PAUSE_ID;
}