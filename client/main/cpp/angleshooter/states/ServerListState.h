#pragma once

class ServerListState final : public State {
	WidgetContainer gui;
	sf::Text remainingPorts;

public:
	static const Identifier SERVER_LIST_ID;
	ServerListState();
	void init() override;
	void seekThread();
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};