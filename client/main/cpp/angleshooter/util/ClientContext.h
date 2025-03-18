#pragma once
#include "../resources/ResourceHolders.h"

class WorldRenderer;
class World;
class AssetBank;
class StateManager;
class OptionsManager;
class InputManager;
class AudioManager;
namespace sf {
	class RenderTexture;
	class RenderWindow;
}

class ClientContext {
	static ClientContext* instance;
	
protected:
	~ClientContext() = default;
	
public:
	static ClientContext* get();
	static void set(ClientContext* instance);
	[[nodiscard]] virtual sf::RenderWindow* getWindow() = 0;
	[[nodiscard]] virtual sf::RenderTexture* getRenderTexture() = 0;
	[[nodiscard]] virtual AudioManager* getAudioManager() = 0;
	[[nodiscard]] virtual InputManager* getInputManager() = 0;
	[[nodiscard]] virtual OptionsManager* getOptionsManager() = 0;
	[[nodiscard]] virtual StateManager* getStateManager() = 0;
	[[nodiscard]] virtual FontHolder* getFontHolder() = 0;
	[[nodiscard]] virtual TextureHolder* getTextureHolder() = 0;
	[[nodiscard]] virtual SoundHolder* getSoundHolder() = 0;
	[[nodiscard]] virtual ShaderHolder* getShaderHolder() = 0;
	[[nodiscard]] virtual World* getWorld() = 0;
	[[nodiscard]] virtual WorldRenderer* getWorldRenderer() = 0;
};