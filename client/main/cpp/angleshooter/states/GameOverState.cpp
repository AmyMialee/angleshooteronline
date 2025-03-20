#include "PreCompiledClient.h"
#include "GameOverState.h"

const Identifier GameOverState::GAME_OVER_ID("gameover");

void GameOverState::init() {
	const auto exitButton = std::make_shared<Button>();
	const auto center = sf::Vector2f{ClientContext::get()->getWindow()->getView().getCenter().x / 2 - 100, 450};
	exitButton->setPosition(center);
	exitButton->setText("Exit");
	exitButton->setCallback([this] {
		requestStackClear();
		requestStackPush(MenuState::getId());
	});
	gui.pack(exitButton);
}

void GameOverState::loadAssets() {}

void GameOverState::render(float deltaTime) {
	static sf::RectangleShape backgroundShape;
	static std::once_flag flag;
	std::call_once(flag, [&] {
		backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
		backgroundShape.setSize(ClientContext::get()->getWindow()->getView().getSize());
	});
	auto& texture = *ClientContext::get()->getRenderTexture();
	texture.draw(backgroundShape);
	texture.setView(texture.getDefaultView());
	texture.draw(gui);
	// const auto center = sf::Vector2f{texture.getView().getCenter().x, 164.f};
	// auto offset = 0.f;
	// auto data = ClientWorld::get().getPlayerData();
	// for (auto it = data.begin(); it != data.end(); ++it) {
		// auto playerData = it->second;
		// auto text = sf::Text(FontHolder::getInstance().getDefault(), std::to_string(playerData.getScore()), 56);
		// text.setPosition(center + sf::Vector2f{10.f, offset + 0.f});
		// text.setFillColor(sf::Color::Cyan);
		// ClientContext::get()->getWindow()->draw(text);
		// text.setPosition(center + sf::Vector2f{8.f, offset + -5.f});
		// text.setCharacterSize(48);
		// text.setFillColor(sf::Color::White);
		// ClientContext::get()->getWindow()->draw(text);
		// offset += 56;
	// }
}

bool GameOverState::shouldRenderNextState() const {
	return true;
}

bool GameOverState::tick(float deltaTime) {
	gui.tick(deltaTime);
	return false;
}

bool GameOverState::handleEvent(const sf::Event& event) {
	if (const auto* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
		if (keyPressed->scancode == sf::Keyboard::Scan::Escape) {
			requestStackClear();
			requestStackPush(MenuState::getId());
			return false;
		}
	}
	gui.handleEvent(event);
	return false;
}

Identifier GameOverState::getId() {
	return GAME_OVER_ID;
}

Identifier GameOverState::getStateId() {
	return GAME_OVER_ID;
}