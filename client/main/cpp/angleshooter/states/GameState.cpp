#include "PreCompiledClient.h"
#include "GameState.h"

const Identifier GameState::GAME_ID("game");

void GameState::init() {
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientContext::get()->getWorld()->init();
	AudioManager::get().playMusic(GAME_MUSIC);
}

void GameState::loadAssets() {
	TextureHolder::get().load(Map::DEFAULT_TILE);
	TextureHolder::get().load(Identifier("cobble.png"));
	TextureHolder::get().load(Identifier("player.png"));
	TextureHolder::get().load(Identifier("player2.png"));
	TextureHolder::get().load(Identifier("bullet.png"));
	TextureHolder::get().load(Identifier("bullet2.png"));
	SoundHolder::get().load(Identifier("bullet.ogg"));
	SoundHolder::get().load(Identifier("hurt.ogg"));
	SoundHolder::get().load(Identifier("explode.ogg"));
}

void GameState::render(float deltaTime) {
	auto& window = *ClientContext::get()->getRenderTexture();
	window.setView(window.getDefaultView());
	ClientContext::get()->getWorldRenderer()->render(deltaTime);
}

bool GameState::shouldRenderNextState() const {
	return false;
}

bool GameState::tick(float deltaTime) {
	ClientContext::get()->getWorld()->tick(deltaTime);
	for (const auto& [first, second] : ClientContext::get()->getWorld()->getPlayerData()) {
		if (second.getScore() >= 9) this->requestStackPush(GameOverState::getId());
	}
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) requestStackPush(PauseState::getId());
	}
	return false;
}

Identifier GameState::getId() {
	return GAME_ID;
}

Identifier GameState::getStateId() {
	return GAME_ID;
}