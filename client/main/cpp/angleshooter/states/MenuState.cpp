#include "PreCompiledClient.h"
#include "MenuState.h"

#include "JoiningState.h"

const Identifier MenuState::MENU_ID("menu");
const Identifier MenuState::MENU_TEXTURE("main_menu.png");

void MenuState::init() {
	const auto playButton = std::make_shared<Button>();
	playButton->setPosition({100, 250});
	playButton->setText("Play");
	playButton->setCallback([this] {
		requestStackPop();
		requestStackPush(GameState::getId());
	});
	const auto settingsButton = std::make_shared<Button>();
	settingsButton->setPosition({100, 300});
	settingsButton->setText("Settings");
	settingsButton->setCallback([this] {
		requestStackPush(SettingsState::getId());
	});
	const auto exitButton = std::make_shared<Button>();
	exitButton->setPosition({100, 350});
	exitButton->setText("Exit");
	exitButton->setCallback([this] {
		requestStackPop();
	});
	const auto setIpButton = std::make_shared<Button>();
	setIpButton->setPosition({100, 400});
	setIpButton->setText("Set IP");
	setIpButton->setCallback([this] {
		requestStackPop();
		requestStackPush(JoiningState::getId());
	});
	gui.pack(playButton);
	gui.pack(settingsButton);
	gui.pack(exitButton);
	gui.pack(setIpButton);
	AudioManager::get().playMusic(Identifier("backgroundmusic.ogg"));
}

void MenuState::loadAssets() {
	TextureHolder::getInstance().load(MENU_TEXTURE);
	TextureHolder::getInstance().load(Button::defaultTexture);
	TextureHolder::getInstance().load(Button::pressedTexture);
	TextureHolder::getInstance().load(Button::selectedTexture);
	SoundHolder::getInstance().load(Button::buttonSound);
}

void MenuState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		Util::centre(background);
		background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		background.setScale({2.f, 2.f});
	});
	auto& window = AngleShooterClient::get().renderTexture;
	window.setView(window.getDefaultView());
	window.draw(background);
	window.draw(gui);
}

bool MenuState::shouldRenderNextState() const {
	return false;
}

bool MenuState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return false;
}

bool MenuState::handleEvent(const sf::Event& event) {
	gui.handleEvent(event);
	return false;
}

Identifier MenuState::getId() {
	return MENU_ID;
}

Identifier MenuState::getStateId() {
	return MENU_ID;
}