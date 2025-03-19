#pragma once

class AngleShooterClient final : ClientContext {
	static AngleShooterClient* instance;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	World world;
	WorldRenderer worldRenderer;
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
	const static Identifier BACKGROUND_MUSIC;
	AngleShooterClient();
	void run();

	[[nodiscard]] sf::RenderWindow* getWindow() override;
	[[nodiscard]] sf::RenderTexture* getRenderTexture() override;
	[[nodiscard]] World* getWorld() override;
	[[nodiscard]] WorldRenderer* getWorldRenderer() override;
};