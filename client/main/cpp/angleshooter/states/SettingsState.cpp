#include "PreCompiledClient.h"
#include "SettingsState.h"

const Identifier SettingsState::SETTINGS_ID("settings");

void SettingsState::init() {
	gui.clear();
	constexpr auto offset = 36.f;
	const auto masterSlider = std::make_shared<Slider>();
	masterSlider->setPosition({80.f, 100.f + offset * 3});
	masterSlider->setTextFunction([](double value) { return "Master Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	masterSlider->setConstantCallback(Consumer<double>([this](double value) {
		AudioManager::get().setMusicVolume(OptionsManager::get().getMusicVolume() * value);
		AudioManager::get().setSoundVolume(OptionsManager::get().getSoundVolume() * value);
	}));
	masterSlider->setFinalCallback(Consumer<double>([this](double value) { OptionsManager::get().setMasterVolume(value); }));
	masterSlider->setValue(OptionsManager::get().getMasterVolume());
	gui.pack(masterSlider);

	const auto soundSlider = std::make_shared<Slider>();
	soundSlider->setPosition({80.f, 100.f + offset * 4});
	soundSlider->setTextFunction([](double value) { return "Sound Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	soundSlider->setConstantCallback(Consumer<double>([this](double value) { AudioManager::get().setSoundVolume(value * OptionsManager::get().getMasterVolume()); }));
	soundSlider->setFinalCallback(Consumer<double>([this](double value) { OptionsManager::get().setSoundVolume(value); }));
	soundSlider->setValue(OptionsManager::get().getSoundVolume());
	gui.pack(soundSlider);

	const auto musicSlider = std::make_shared<Slider>();
	musicSlider->setPosition({80.f, 100.f + offset * 5});
	musicSlider->setTextFunction([](double value) { return "Music Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	musicSlider->setConstantCallback(Consumer<double>([this](double value) { AudioManager::get().setMusicVolume(value * OptionsManager::get().getMasterVolume()); }));
	musicSlider->setFinalCallback(Consumer<double>([this](double value) { OptionsManager::get().setMusicVolume(value); }));
	musicSlider->setValue(OptionsManager::get().getMusicVolume());
	gui.pack(musicSlider);

	const auto fpsSlider = std::make_shared<Slider>();
	fpsSlider->setPosition({80.f, 100.f + offset * 7});
	fpsSlider->setTextFunction([](double value) { return "FPS : " + std::to_string(static_cast<int>(value * 144.)); });
	fpsSlider->setFinalCallback(Consumer<double>([this](double value) { OptionsManager::get().setFps(static_cast<int>(value * 144.)); }));
	fpsSlider->setValue(OptionsManager::get().getFps() / 144.);
	gui.pack(fpsSlider);

	const auto ipButton = std::make_shared<IpButton>();
	ipButton->setPosition({300.f, 100.f + offset * 3});
	auto ipText = [&](const std::string& ip) { return "IP: " + ip; };
	ipButton->setText(ipText(OptionsManager::get().getIp()));
	ipButton->setTextFunction(ipText);
	gui.pack(ipButton);

	const auto nameButton = std::make_shared<NameButton>();
	nameButton->setPosition({300.f, 100.f + offset * 4});
	auto nameText = [&](const std::string& name) { return "Name: " + name; };
	nameButton->setText(nameText(OptionsManager::get().getName()));
	nameButton->setTextFunction(nameText);
	gui.pack(nameButton);

	auto makeControlsButton = [this](sf::Vector2f pos, const std::string& text, Keybinding* keybinding) {
		const auto controlButton = std::make_shared<Button>();
		controlButton->setToggle(true);
		controlButton->setPosition(pos);
		controlButton->setText(text + " : " + getDescription(keybinding->getKey()));
		controlButton->setCallback([this, keybinding] {
			InputManager::get().setSelectedKeybinding(keybinding);
		});
		gui.pack(controlButton);
	};
	makeControlsButton({520.f, 100.f + offset * 3}, "Up", InputManager::get().getUp());
	makeControlsButton({520.f, 100.f + offset * 4}, "Down", InputManager::get().getDown());
	makeControlsButton({520.f, 100.f + offset * 5}, "Left", InputManager::get().getLeft());
	makeControlsButton({520.f, 100.f + offset * 6}, "Right", InputManager::get().getRight());
	makeControlsButton({520.f, 100.f + offset * 7}, "Fire", InputManager::get().getFire());

	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({300.f, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
}

void SettingsState::loadAssets() {}

void SettingsState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MenuState::MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		background.setColor({200, 255, 255, 255});
		Util::centre(background);
		background.setPosition(AngleShooterClient::get().renderTexture.getView().getSize() / 2.f);
		background.setScale({2.f, 2.f});
	});
	auto& texture = AngleShooterClient::get().renderTexture;
	texture.draw(background);
	texture.draw(gui);
}

bool SettingsState::shouldRenderNextState() const {
	return false;
}

bool SettingsState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return false;
}

bool SettingsState::handleEvent(const sf::Event& event) {
	if (const auto keybinding = InputManager::get().getSelectedKeybinding()) {
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode != sf::Keyboard::Scan::Escape && keyPressed->scancode != sf::Keyboard::Scan::Enter && keyPressed->scancode != sf::Keyboard::Scan::Space) {
				keybinding->rebind(keyPressed->scancode);
				InputManager::get().setSelectedKeybinding(nullptr);
				const auto selected = gui.getSelectedChild();
				this->init();
				gui.setSelected(selected);
				gui.selectNext();
				gui.selectPrevious();
				return false;
			}
		}
	}
	gui.handleEvent(event);
	return false;
}

Identifier SettingsState::getId() {
	return SETTINGS_ID;
}

Identifier SettingsState::getStateId() {
	return SETTINGS_ID;
}