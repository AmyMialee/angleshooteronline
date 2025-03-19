#pragma once

class ClientContext {
	static ClientContext* instance;
	
protected:
	~ClientContext() = default;
	
public:
	static ClientContext* get();
	static void set(ClientContext* instance);
	[[nodiscard]] virtual sf::RenderWindow* getWindow() = 0;
	[[nodiscard]] virtual sf::RenderTexture* getRenderTexture() = 0;
	[[nodiscard]] virtual World* getWorld() = 0;
	[[nodiscard]] virtual WorldRenderer* getWorldRenderer() = 0;
};