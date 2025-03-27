#pragma once

class AngleShooterClient final {
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
	AngleShooterClient();
	void run();

	[[nodiscard]] static AngleShooterClient* get();
	[[nodiscard]] sf::RenderWindow* getWindow();
	[[nodiscard]] sf::RenderTexture* getRenderTexture();
};