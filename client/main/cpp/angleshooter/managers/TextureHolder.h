#pragma once

class TextureHolder final : public ResourceHolder<sf::Texture>, public Singleton<TextureHolder> {
	explicit TextureHolder();
	std::unique_ptr<sf::Texture>* createDefaultTexture();
};