#pragma once

class GameState final : public State {
public:
	inline static std::unordered_map<uint16_t, ScoreEntry> SCORES;
	static const Identifier GAME_ID;
	GameState();
	void init() override;
	void destroy() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	static void refreshScores();
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};
