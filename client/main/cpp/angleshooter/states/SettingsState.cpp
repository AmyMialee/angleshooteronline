#include "PreCompiledClient.h"
#include "SettingsState.h"

const Identifier SettingsState::SETTINGS_ID("settings");

void SettingsState::init() {
	gui.clear();
	constexpr auto offset = 36.f;
	const auto masterSlider = std::make_shared<Slider>();
	masterSlider->setPosition({80.f, 100.f + offset * 3});
	masterSlider->setTextFunction([](double value) { return "Master Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	masterSlider->setConstantCallback([this](double value) {
		ClientContext::get()->getAudioManager()->setMusicVolume(ClientContext::get()->getOptionsManager()->getMusicVolume() * value);
		ClientContext::get()->getAudioManager()->setSoundVolume(ClientContext::get()->getOptionsManager()->getSoundVolume() * value);
	});
	masterSlider->setFinalCallback([this](double value) { ClientContext::get()->getOptionsManager()->setMasterVolume(value); });
	masterSlider->setValue(ClientContext::get()->getOptionsManager()->getMasterVolume());
	gui.pack(masterSlider);
	
	const auto soundSlider = std::make_shared<Slider>();
	soundSlider->setPosition({80.f, 100.f + offset * 4});
	soundSlider->setTextFunction([](double value) { return "Sound Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	soundSlider->setConstantCallback([this](double value) { ClientContext::get()->getAudioManager()->setSoundVolume(value * ClientContext::get()->getOptionsManager()->getMasterVolume()); });
	soundSlider->setFinalCallback([this](double value) { ClientContext::get()->getOptionsManager()->setSoundVolume(value); });
	soundSlider->setValue(ClientContext::get()->getOptionsManager()->getSoundVolume());
	gui.pack(soundSlider);
	
	const auto musicSlider = std::make_shared<Slider>();
	musicSlider->setPosition({80.f, 100.f + offset * 5});
	musicSlider->setTextFunction([](double value) { return "Music Volume : " + std::to_string(static_cast<int>(value * 100.)); });
	musicSlider->setConstantCallback([this](double value) { ClientContext::get()->getAudioManager()->setMusicVolume(value * ClientContext::get()->getOptionsManager()->getMasterVolume()); });
	musicSlider->setFinalCallback([this](double value) { ClientContext::get()->getOptionsManager()->setMusicVolume(value); });
	musicSlider->setValue(ClientContext::get()->getOptionsManager()->getMusicVolume());
	gui.pack(musicSlider);
	
	const auto fpsSlider = std::make_shared<Slider>();
	fpsSlider->setPosition({80.f, 100.f + offset * 7});
	fpsSlider->setTextFunction([](double value) { return "FPS : " + std::to_string(static_cast<int>(value * 144.)); });
	fpsSlider->setFinalCallback([this](double value) { ClientContext::get()->getOptionsManager()->setFps(static_cast<int>(value * 144.)); });
	fpsSlider->setValue(ClientContext::get()->getOptionsManager()->getFps() / 144.);
	gui.pack(fpsSlider);

	auto makeControlsButton = [this](sf::Vector2f pos, const std::string& text, Keybinding* keybinding) {
		const auto controlButton = std::make_shared<Button>();
		controlButton->setToggle(true);
		controlButton->setPosition(pos);
		controlButton->setText(text + " : " + getDescription(keybinding->getKey()));
		controlButton->setCallback([this, keybinding] {
			ClientContext::get()->getInputManager()->setSelectedKeybinding(keybinding);
		});
		gui.pack(controlButton);
	};
	makeControlsButton({300.f, 100.f + offset * 3}, "Up", ClientContext::get()->getInputManager()->getUp());
	makeControlsButton({300.f, 100.f + offset * 4}, "Down", ClientContext::get()->getInputManager()->getDown());
	makeControlsButton({300.f, 100.f + offset * 5}, "Left", ClientContext::get()->getInputManager()->getLeft());
	makeControlsButton({300.f, 100.f + offset * 6}, "Right", ClientContext::get()->getInputManager()->getRight());
	makeControlsButton({300.f, 100.f + offset * 7}, "Fire", ClientContext::get()->getInputManager()->getFire());
	
	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({300.f, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
}

void SettingsState::loadAssets() {}

void SettingsState::render(float deltaTime) {
	static sf::Sprite background(ClientContext::get()->getTextureHolder()->get(MenuState::MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		background.setColor({200, 255, 255, 255});
		Util::centre(background);
		background.setPosition(ClientContext::get()->getRenderTexture()->getView().getSize() / 2.f);
		background.setScale({2.f, 2.f});
	});
	auto& texture = *ClientContext::get()->getRenderTexture();
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
	if (const auto keybinding = ClientContext::get()->getInputManager()->getSelectedKeybinding()) {
		if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode != sf::Keyboard::Scan::Escape && keyPressed->scancode != sf::Keyboard::Scan::Enter && keyPressed->scancode != sf::Keyboard::Scan::Space) {
				keybinding->rebind(ClientContext::get()->getOptionsManager(), keyPressed->scancode);
				ClientContext::get()->getInputManager()->setSelectedKeybinding(nullptr);
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