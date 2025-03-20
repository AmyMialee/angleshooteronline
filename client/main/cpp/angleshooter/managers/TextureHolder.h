#pragma once

class TextureHolder final : public ResourceHolder<sf::Texture>, public InstancedSingleton<TextureHolder> {
	friend class InstancedSingleton;
	explicit TextureHolder();
	std::unique_ptr<sf::Texture> createDefaultTexture();
};