#include "PreCompiledClient.h"
#include "OnboardingState.h"

const Identifier OnboardingState::ONBOARDING_ID("onboarding");

void OnboardingState::init() {
	const auto masterSlider = std::make_shared<Slider>();
	masterSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 200.f});
	masterSlider->setTextFunction([](double value) {
		return "Master Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	masterSlider->setConstantCallback(Consumer<double>([](double value) {
		AudioManager::get().setMusicVolume(OptionsManager::get().getMusicVolume() * value);
		AudioManager::get().setSoundVolume(OptionsManager::get().getSoundVolume() * value);
	}));
	masterSlider->setFinalCallback(Consumer<double>([](double value) {
		OptionsManager::get().setMasterVolume(value);
	}));
	masterSlider->setValue(OptionsManager::get().getMasterVolume());
	gui.pack(masterSlider);
	const auto musicSlider = std::make_shared<Slider>();
	musicSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 232.f});
	musicSlider->setTextFunction([](double value) {
		return "Music Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	musicSlider->setConstantCallback(Consumer<double>([this](double value) {
		AudioManager::get().setMusicVolume(value * OptionsManager::get().getMasterVolume());
	}));
	musicSlider->setFinalCallback(Consumer<double>([this](double value) {
		OptionsManager::get().setMusicVolume(value);
	}));
	musicSlider->setValue(OptionsManager::get().getMusicVolume());
	gui.pack(musicSlider);
	const auto soundSlider = std::make_shared<Slider>();
	soundSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 264.f});
	soundSlider->setTextFunction([](double value) {
		return "Sound Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	soundSlider->setConstantCallback(Consumer<double>([this](double value) {
		AudioManager::get().setSoundVolume(value * OptionsManager::get().getMasterVolume());
	}));
	soundSlider->setFinalCallback(Consumer<double>([this](double value) {
		OptionsManager::get().setSoundVolume(value);
	}));
	soundSlider->setValue(OptionsManager::get().getSoundVolume());
	gui.pack(soundSlider);
	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
	OptionsManager::get().setOnboarded(true);
	AudioManager::get().playMusic(AngleShooterClient::BACKGROUND_MUSIC);
}

void OnboardingState::loadAssets() {}

void OnboardingState::render(float deltaTime) {
	static sf::Sprite background(TextureHolder::getInstance().get(MenuState::MENU_TEXTURE));
	static std::once_flag flag;
	std::call_once(flag, [&] {
		Util::centre(background);
		background.setPosition(ClientContext::get()->getRenderTexture()->getView().getSize() / 2.f);
		background.setColor({165, 255, 255, 255});
		background.setScale({2.f, 2.f});
	});
	auto& texture = *ClientContext::get()->getRenderTexture();
	texture.draw(background);
	texture.draw(gui);
}

bool OnboardingState::shouldRenderNextState() const {
	return false;
}

bool OnboardingState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return false;
}

bool OnboardingState::handleEvent(const sf::Event& event) {
	gui.handleEvent(event);
	return false;
}

Identifier OnboardingState::getId() {
	return ONBOARDING_ID;
}

Identifier OnboardingState::getStateId() {
	return ONBOARDING_ID;
}