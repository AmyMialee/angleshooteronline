#include "PreCompiledClient.h"
#include "OnboardingState.h"

const Identifier OnboardingState::ONBOARDING_ID("onboarding");

void OnboardingState::init() {
	const auto masterSlider = std::make_shared<Slider>();
	masterSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 200.f});
	masterSlider->setTextFunction([](double value) {
		return "Master Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	masterSlider->setConstantCallback([this](double value) {
		ClientContext::get()->getAudioManager()->setMusicVolume(ClientContext::get()->getOptionsManager()->getMusicVolume() * value);
		ClientContext::get()->getAudioManager()->setSoundVolume(ClientContext::get()->getOptionsManager()->getSoundVolume() * value);
	});
	masterSlider->setFinalCallback([this](double value) {
		ClientContext::get()->getOptionsManager()->setMasterVolume(value);
	});
	masterSlider->setValue(ClientContext::get()->getOptionsManager()->getMasterVolume());
	gui.pack(masterSlider);
	const auto musicSlider = std::make_shared<Slider>();
	musicSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 232.f});
	musicSlider->setTextFunction([](double value) {
		return "Music Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	musicSlider->setConstantCallback([this](double value) {
		ClientContext::get()->getAudioManager()->setMusicVolume(value * ClientContext::get()->getOptionsManager()->getMasterVolume());
	});
	musicSlider->setFinalCallback([this](double value) {
		ClientContext::get()->getOptionsManager()->setMusicVolume(value);
	});
	musicSlider->setValue(ClientContext::get()->getOptionsManager()->getMusicVolume());
	gui.pack(musicSlider);
	const auto soundSlider = std::make_shared<Slider>();
	soundSlider->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 264.f});
	soundSlider->setTextFunction([](double value) {
		return "Sound Volume : " + std::to_string(static_cast<int>(value * 100.));
	});
	soundSlider->setConstantCallback([this](double value) {
		ClientContext::get()->getAudioManager()->setSoundVolume(value * ClientContext::get()->getOptionsManager()->getMasterVolume());
	});
	soundSlider->setFinalCallback([this](double value) {
		ClientContext::get()->getOptionsManager()->setSoundVolume(value);
	});
	soundSlider->setValue(ClientContext::get()->getOptionsManager()->getSoundVolume());
	gui.pack(soundSlider);
	const auto backButton = std::make_shared<Button>();
	backButton->setPosition({ClientContext::get()->getRenderTexture()->getView().getSize().x / 2 - 100, 475.f});
	backButton->setText("Back");
	backButton->setCallback([this] { requestStackPop(); });
	gui.pack(backButton);
	ClientContext::get()->getOptionsManager()->setOnboarded(true);
	ClientContext::get()->getAudioManager()->playMusic(AngleShooterClient::BACKGROUND_MUSIC);
}

void OnboardingState::loadAssets() {}

void OnboardingState::render(float deltaTime) {
	static sf::Sprite background(ClientContext::get()->getTextureHolder()->get(MenuState::MENU_TEXTURE));
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