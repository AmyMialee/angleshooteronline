#include "PreCompiledClient.h"
#include "GameState.h"

const Identifier GameState::GAME_ID("game");

void GameState::init() {
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientWorld::get().init();
	AudioManager::get().playMusic(GAME_MUSIC);
}

void GameState::loadAssets() {
	TextureHolder::getInstance().load(Map::DEFAULT_TILE);
	TextureHolder::getInstance().load(Identifier("cobble.png"));
	TextureHolder::getInstance().load(Identifier("player.png"));
	TextureHolder::getInstance().load(Identifier("player2.png"));
	TextureHolder::getInstance().load(Identifier("bullet.png"));
	TextureHolder::getInstance().load(Identifier("bullet2.png"));
	SoundHolder::getInstance().load(Identifier("bullet.ogg"));
	SoundHolder::getInstance().load(Identifier("hurt.ogg"));
	SoundHolder::getInstance().load(Identifier("explode.ogg"));
}

void GameState::render(float deltaTime) {
	auto& window = *ClientContext::get()->getRenderTexture();
	window.setView(window.getDefaultView());
	WorldRenderer::get().render(deltaTime);
}

bool GameState::shouldRenderNextState() const {
	return false;
}

bool GameState::tick(float deltaTime) {
	ClientWorld::get().tick(deltaTime);
	// for (const auto& [first, second] : ClientWorld::get().getPlayerData()) {
		// if (second.getScore() >= 9) this->requestStackPush(GameOverState::getId());
	// }
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