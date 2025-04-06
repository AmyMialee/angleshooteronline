#pragma once

class PauseState final : public State {
	WidgetContainer gui;

public:
	static const Identifier PAUSE_ID;
	void init() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};