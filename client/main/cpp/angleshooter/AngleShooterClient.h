#pragma once

class AngleShooterClient final : ClientContext {
	static AngleShooterClient* instance;
	sf::RenderWindow window;
	sf::RenderTexture renderTexture;
	AudioManager audio;
	InputManager inputManager;
	OptionsManager options;
	StateManager states;
	TextureHolder textureHolder;
	FontHolder fontHolder;
	SoundHolder soundHolder;
	ShaderHolder shaderHolder;
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
	[[nodiscard]] AudioManager* getAudioManager() override;
	[[nodiscard]] InputManager* getInputManager() override;
	[[nodiscard]] OptionsManager* getOptionsManager() override;
	[[nodiscard]] StateManager* getStateManager() override;
	[[nodiscard]] FontHolder* getFontHolder() override;
	[[nodiscard]] TextureHolder* getTextureHolder() override;
	[[nodiscard]] SoundHolder* getSoundHolder() override;
	[[nodiscard]] ShaderHolder* getShaderHolder() override;
	[[nodiscard]] World* getWorld() override;
	[[nodiscard]] WorldRenderer* getWorldRenderer() override;
};