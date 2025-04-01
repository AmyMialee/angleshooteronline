#pragma once

class JoiningState final : public State {
public:
	static const Identifier JOINING_ID;
	std::string ipAddress;
	void init() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};