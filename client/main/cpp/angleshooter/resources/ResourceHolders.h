#pragma once
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "ResourceHolder.h"
#include "ShaderHolder.h"

typedef ResourceHolder<sf::Texture> TextureHolder;
typedef ResourceHolder<sf::Font> FontHolder;
typedef ResourceHolder<sf::SoundBuffer> SoundHolder;
typedef ShaderHolder ShaderHolder;
class ResourceHolders {
public:
	static TextureHolder makeTextureHolder();
	static FontHolder makeFontHolder();
	static SoundHolder makeSoundHolder();
	static ShaderHolder makeShaderHolder();
};