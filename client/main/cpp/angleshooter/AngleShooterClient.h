#pragma once

class AngleShooterClient final : ClientContext {
	static AngleShooterClient* instance;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	double tps;
	double fps;
    sf::Text tpsText;
    sf::Text fpsText;

	void tick(float deltaTime);
	void render(float deltaTime);
	void registerStates();
	void loadAssets();

public:
	static double timePerTick;
	inline const static auto BACKGROUND_MUSIC = Identifier("backgroundmusic.ogg");
	AngleShooterClient();
	void run();

	[[nodiscard]] sf::RenderWindow* getWindow() override;
	[[nodiscard]] sf::RenderTexture* getRenderTexture() override;
};