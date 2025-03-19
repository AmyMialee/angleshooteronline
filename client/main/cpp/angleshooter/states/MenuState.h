#pragma once

class MenuState final : public State {
	WidgetContainer gui;

public:
	static const Identifier MENU_ID;
	static const Identifier MENU_TEXTURE;
	void init() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};