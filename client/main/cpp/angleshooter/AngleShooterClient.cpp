#include "PreCompiledClient.h"
#include "AngleShooterClient.h"

#include "resources/FontHolder.h"

double AngleShooterClient::timePerTick = 1. / 60.;

AngleShooterClient::AngleShooterClient() :
	ClientContext(),
	window(sf::VideoMode({1920, 1080}), "Angle Shooter", sf::Style::Titlebar | sf::Style::Close),
	renderTexture({960, 540}),
	tps(static_cast<int>(1 / timePerTick)),
	fps(144),
	tpsText(FontHolder::get().getDefault(), "", 12),
	fpsText(FontHolder::get().getDefault(), "", 12) {
	set(this);
	window.clear();
	window.setKeyRepeatEnabled(false);
	registerStates();
	loadAssets();
	tpsText.setPosition({4.f, 4.f + 14});
	fpsText.setPosition({4.f, 4.f});
	StateManager::get().push(SplashState::getId());
	OptionsManager::get().loadFromFile();
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
		InputManager::get().handleInput(window);
		if (tickTime > 1.) {
			Logger::warn("AngleShooter::run: Lagging behind by " + Util::toRoundedString(tickTime / timePerTick, 2) + " ticks (" + Util::toRoundedString(tickTime, 2) + " seconds), skipping ahead");
			tickTime = timePerTick;
		}
		while (tickTime >= timePerTick) {
			tickTime -= timePerTick;
			tick(static_cast<float>(tickTime / timePerTick));
			++ticks;
		}
		if (const auto timePerFrame = OptionsManager::get().getTimePerFrame(); frameTime >= timePerFrame) {
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
	AudioManager::get().tick(deltaTime);
	StateManager::get().tick(deltaTime);
}

void AngleShooterClient::render(float deltaTime) {
	window.clear();
	renderTexture.clear();
	StateManager::get().render(deltaTime);
	renderTexture.display();
	sf::Sprite sprite(renderTexture.getTexture());
	sprite.setScale({2.f, 2.f});
	window.draw(sprite);
	if (OptionsManager::get().isDebugEnabled()) {
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
	if (StateManager::get().getStateId() == GameState::getId()) {
		const auto center = sf::Vector2f{window.getView().getCenter().x, 16.f};
		auto offset = 0.f;
		for (auto data = this->world.getPlayerData(); const auto& playerData : data | std::views::values) {
			auto text = sf::Text(FontHolder::get().getDefault(), std::to_string(playerData.getScore()), 56);
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
	StateManager::get().registerState<SplashState>(SplashState::getId());
	StateManager::get().registerState<MenuState>(MenuState::getId());
	StateManager::get().registerState<SettingsState>(SettingsState::getId());
	StateManager::get().registerState<OnboardingState>(OnboardingState::getId());
	StateManager::get().registerState<GameState>(GameState::getId());
	StateManager::get().registerState<PauseState>(PauseState::getId());
	StateManager::get().registerState<GameOverState>(GameOverState::getId());
}

void AngleShooterClient::loadAssets() {
	StateManager::get().loadAssets();
}

sf::RenderWindow* AngleShooterClient::getWindow() {
	return &window;
}

sf::RenderTexture* AngleShooterClient::getRenderTexture() {
	return &renderTexture;
}

World* AngleShooterClient::getWorld() {
	return &world;
}