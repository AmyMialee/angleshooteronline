#include "PreCompiledClient.h"
#include "GameState.h"

const Identifier GameState::GAME_ID("game");

GameState::GameState() : State() {}

void GameState::init() {
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientWorld::get().init();
	AudioManager::get().playMusic(GAME_MUSIC);
	// Converting IP Address from string to uint32_t code from Deepseek
	std::vector<uint8_t> octets;
	std::istringstream iss(OptionsManager::get().getIp());
	std::string token;
	while (std::getline(iss, token, '.')) {
		const int octet = std::stoi(token);
		octets.push_back(static_cast<uint8_t>(octet));
	}
	const uint32_t ip = octets[0] << 24 | octets[1] << 16 | octets[2] << 8 | octets[3];
	// External code end
	AngleShooterClient::get().connect(sf::IpAddress(ip));
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
	auto& window = AngleShooterClient::get().renderTexture;
	window.setView(window.getDefaultView());
	WorldRenderer::get().render(deltaTime);
	AngleShooterClient::get().window.setView(AngleShooterClient::get().window.getDefaultView());
}

bool GameState::shouldRenderNextState() const {
	return false;
}

bool GameState::tick(float deltaTime) {
	ClientWorld::get().tick(deltaTime);
	return false;
}

bool GameState::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) if (keyPressed->scancode == sf::Keyboard::Scan::Escape) requestStackPush(PauseState::getId());
	return false;
}

Identifier GameState::getId() {
	return GAME_ID;
}

Identifier GameState::getStateId() {
	return GAME_ID;
}