#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

double AngleShooterClient::timePerTick = 1. / 60.;

const Identifier AngleShooterClient::BACKGROUND_MUSIC = Identifier("backgroundmusic.ogg");

AngleShooterClient::AngleShooterClient() :
	ClientContext(),
	window(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Titlebar | sf::Style::Close),
	renderTexture({960, 540}),
	textureHolder(ResourceHolders::makeTextureHolder()),
	fontHolder(ResourceHolders::makeFontHolder()),
	soundHolder(ResourceHolders::makeSoundHolder()),
	shaderHolder(ResourceHolders::makeShaderHolder()),
	tps(static_cast<int>(1 / timePerTick)),
	fps(144),
	tpsText(fontHolder.getDefault(), "", 12),
	fpsText(fontHolder.getDefault(), "", 12) {
	set(this);
	window.clear();
	window.setKeyRepeatEnabled(false);
	registerStates();
	loadAssets();
	tpsText.setPosition({4.f, 4.f + 14});
	fpsText.setPosition({4.f, 4.f});
	states.push(SplashState::getId());
	options.loadFromFile();
}

void AngleShooterClient::run() {
	this->world.init();
	sf::Clock deltaClock;
	auto tickTime = 0.;
	auto frameTime = 0.;
	auto secondTime = 0.;
	auto ticks = 0;
	auto frames = 0;
	while (window.isOpen()) {
		const auto deltaTime = deltaClock.restart().asSeconds();
		tickTime += deltaTime;
		frameTime += deltaTime;
		secondTime += deltaTime;
		inputManager.handleInput(window, states);
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / timePerTick, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
			tickTime = timePerTick;
		}
		while (tickTime >= timePerTick) {
			tickTime -= timePerTick;
			tick(static_cast<float>(tickTime / timePerTick));
			++ticks;
		}
		if (const auto timePerFrame = options.getTimePerFrame(); frameTime >= timePerFrame) {
			frameTime -= timePerFrame;
			render(static_cast<float>(tickTime / timePerTick));
			while (frameTime >= timePerFrame) frameTime -= timePerFrame;
			++frames;
		}
		if (secondTime >= .1f) {
			tps = tps * .6 + .4 * (ticks / secondTime);
			fps = fps * .6 + .4 * (frames / secondTime);
			tpsText.setString("TPS: " + Util::toRoundedString(tps, 2));
			fpsText.setString("FPS: " + Util::toRoundedString(fps, 2));
			ticks = 0;
			frames = 0;
			secondTime = 0;
		}
	}
}

void AngleShooterClient::tick(float deltaTime) {
	audio.tick(deltaTime);
	states.tick(deltaTime);
}

void AngleShooterClient::render(float deltaTime) {
	window.clear();
	renderTexture.clear();
	states.render(deltaTime);
	renderTexture.display();
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setScale({2.f, 2.f});
	window.draw(sprite);
	if (this->options.areHitboxesEnabled()) {
		fpsText.setPosition(fpsText.getPosition() + sf::Vector2f{1.f, 1.f});
		tpsText.setPosition(tpsText.getPosition() + sf::Vector2f{1.f, 1.f});
		fpsText.setFillColor(sf::Color::Black);
		tpsText.setFillColor(sf::Color::Black);
		window.draw(fpsText);
		window.draw(tpsText);
		fpsText.setPosition(fpsText.getPosition() - sf::Vector2f{1.f, 1.f});
		tpsText.setPosition(tpsText.getPosition() - sf::Vector2f{1.f, 1.f});
		fpsText.setFillColor(sf::Color::White);
		tpsText.setFillColor(sf::Color::White);
		window.draw(fpsText);
		window.draw(tpsText);
	}
	if (states.getStateId() == GameState::getId()) {
		const auto center = sf::Vector2f{window.getView().getCenter().x, 16.f};
		auto offset = 0.f;
		for (auto data = this->world.getPlayerData(); const auto& playerData : data | std::views::values) {
			auto text = sf::Text(fontHolder.getDefault(), std::to_string(playerData.getScore()), 56);
			text.setPosition(center + sf::Vector2f{10.f, offset + 0.f});
			text.setFillColor(sf::Color::Cyan);
			window.draw(text);
			text.setPosition(center + sf::Vector2f{8.f, offset + -5.f});
			text.setCharacterSize(48);
			text.setFillColor(sf::Color::White);
			window.draw(text);
			offset += 56;
		}
	}
	window.display();
}

void AngleShooterClient::registerStates() {
	states.registerState<SplashState>(SplashState::getId());
	states.registerState<MenuState>(MenuState::getId());
	states.registerState<SettingsState>(SettingsState::getId());
	states.registerState<OnboardingState>(OnboardingState::getId());
	states.registerState<GameState>(GameState::getId());
	states.registerState<PauseState>(PauseState::getId());
	states.registerState<GameOverState>(GameOverState::getId());
}

void AngleShooterClient::loadAssets() {
	states.loadAssets();
}

sf::RenderWindow* AngleShooterClient::getWindow() {
	return &window;
}

sf::RenderTexture* AngleShooterClient::getRenderTexture() {
	return &renderTexture;
}

AudioManager* AngleShooterClient::getAudioManager() {
	return &audio;
}

InputManager* AngleShooterClient::getInputManager() {
	return &inputManager;
}

OptionsManager* AngleShooterClient::getOptionsManager() {
	return &options;
}

StateManager* AngleShooterClient::getStateManager() {
	return &states;
}

FontHolder* AngleShooterClient::getFontHolder() {
	return &fontHolder;
}

TextureHolder* AngleShooterClient::getTextureHolder() {
	return &textureHolder;
}

SoundHolder* AngleShooterClient::getSoundHolder() {
	return &soundHolder;
}

ShaderHolder* AngleShooterClient::getShaderHolder() {
	return &shaderHolder;
}

World* AngleShooterClient::getWorld() {
	return &world;
}
WorldRenderer* AngleShooterClient::getWorldRenderer() {
	return &worldRenderer;
}