#include "PreCompiledClient.h"
#include "GameState.h"

const Identifier GameState::GAME_ID("game");

GameState::GameState() : State() {}

void GameState::init() {
	SCORES.clear();
	static const auto GAME_MUSIC = Identifier("gamemusic.ogg");
	ClientWorld::get().init();
	AudioManager::get().playMusic(GAME_MUSIC);
	AngleShooterClient::get().connect(AngleShooterClient::get().foundAddress);
}

void GameState::destroy() {
	AngleShooterClient::get().disconnect();
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
	window.setView(window.getDefaultView());
	for (const auto& score : SCORES | std::views::values) {
		static sf::Sprite playerSprite(TextureHolder::getInstance().get(Identifier("player.png")));
		static sf::Text text(FontHolder::getInstance().getDefault());
		static std::once_flag flag;
		std::call_once(flag, [&] {
			Util::centre(playerSprite);
			const auto textureSize = playerSprite.getTexture().getSize();
			playerSprite.setScale(sf::Vector2f{32.f, 32.f}.componentWiseDiv({static_cast<float>(textureSize.x), static_cast<float>(textureSize.y)}));
			Util::centre(text);
			text.setCharacterSize(48);
			text.setScale({0.5f, 0.5f});
			text.setFillColor(sf::Color::White);
			text.setOutlineColor(sf::Color::Black);
			text.setOutlineThickness(2.f);
		});
		const auto pos = sf::Vector2f({22, window.getView().getSize().y / 2 - SCORES.size() * (36 / 2) + score.yCurrent * 36.f});
		playerSprite.setPosition(pos);
		playerSprite.setColor(score.colour);
		window.draw(playerSprite);
		text.setString(score.name + ": " + std::to_string(score.score));
		text.setPosition({pos.x + 22, pos.y - 16});
		const auto textColour = sf::Color({static_cast<std::uint8_t>(score.colour.r / 2 + 128), static_cast<std::uint8_t>(score.colour.g / 2 + 128), static_cast<std::uint8_t>(score.colour.b / 2 + 128)});
		text.setFillColor(textColour);
		window.draw(text);
	}
	AngleShooterClient::get().window.setView(AngleShooterClient::get().window.getDefaultView());
}

void GameState::refreshScores() {
	for (const auto& entity : ClientWorld::get().getEntities()) {
		if (entity->getEntityType() != PlayerEntity::ID) continue;
		const auto player = dynamic_cast<PlayerEntity*>(entity.get());
		if (auto it = SCORES.find(player->getId()); it != SCORES.end()) {
			it->second.score = player->score;
		} else {
			SCORES.emplace(player->getId(), ScoreEntry{player->name, player->colour, player->score, 0, 0});
		}
	}
	std::vector<std::pair<uint16_t, uint16_t>> scores;
	for (const auto& [first, second] : SCORES) scores.emplace_back(first, second.score);
	std::ranges::sort(scores, [](const auto& a, const auto& b) { return a.second > b.second; });
	for (auto i = 0; i < SCORES.size(); ++i) {
		if (auto it = SCORES.find(scores[i].first); it != SCORES.end()) it->second.yTarget = static_cast<float>(i);
	}
}

bool GameState::shouldRenderNextState() const {
	return false;
}

bool GameState::tick(float deltaTime) {
	ClientWorld::get().tick(deltaTime);
	WorldRenderer::get().tick(deltaTime);
	for (auto& score : SCORES | std::views::values) {
		if (std::abs(score.yTarget - score.yCurrent) > 0.01f) {
			score.yLastCurrent = score.yCurrent;
			score.yCurrent += (score.yTarget - score.yCurrent) * 0.1f;
			if (std::abs(score.yTarget - score.yCurrent) <= 0.01f) {
				score.yCurrent = score.yTarget;
				score.yLastCurrent = score.yCurrent;
			}
		}
	}
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