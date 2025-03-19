#include "PreCompiledClient.h"
#include "GameState.h"

const Identifier GameState::GAME_ID("game");

void GameState::init() {
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientContext::get()->getWorld()->init();
	ClientContext::get()->getAudioManager()->playMusic(GAME_MUSIC);
}

void GameState::loadAssets() {
	ClientContext::get()->getTextureHolder()->load(Map::DEFAULT_TILE);
	ClientContext::get()->getTextureHolder()->load(Identifier("cobble.png"));
	ClientContext::get()->getTextureHolder()->load(Identifier("player.png"));
	ClientContext::get()->getTextureHolder()->load(Identifier("player2.png"));
	ClientContext::get()->getTextureHolder()->load(Identifier("bullet.png"));
	ClientContext::get()->getTextureHolder()->load(Identifier("bullet2.png"));
	ClientContext::get()->getSoundHolder()->load(Identifier("bullet.ogg"));
	ClientContext::get()->getSoundHolder()->load(Identifier("hurt.ogg"));
	ClientContext::get()->getSoundHolder()->load(Identifier("explode.ogg"));
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