#pragma once
#include "State.h"

class SplashState final : public State {
	static float totalTime;
	float fadeTime = 0;
	void completeSplash();

public:
	static const Identifier TITLE_ID;
	static const Identifier SPLASH_TEXTURE;
	void init() override;
	void loadAssets() override;
	void render(float deltaTime) override;
	[[nodiscard]] bool shouldRenderNextState() const override;
	bool tick(float deltaTime) override;
	bool handleEvent(const sf::Event& event) override;
	Identifier getStateId() override;
	static Identifier getId();
};