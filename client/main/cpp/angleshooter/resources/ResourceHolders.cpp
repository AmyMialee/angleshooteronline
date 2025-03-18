#include "PreCompiledClient.h"
#include "ResourceHolders.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>

ResourceHolder<sf::Texture> ResourceHolders::makeTextureHolder() {
	auto texture = std::make_unique<sf::Texture>();
	sf::Image image({16, 16}, sf::Color::Magenta);
	for (unsigned int x = 0; x < 8; ++x) {
		for (unsigned int y = 0; y < 8; ++y) {
			image.setPixel({x, y}, sf::Color::Black);
		}
	}
	for (unsigned int x = 8; x < 16; ++x) {
		for (unsigned int y = 8; y < 16; ++y) {
			image.setPixel({x, y}, sf::Color::Black);
		}
	}
	if (!texture->loadFromImage(image)) Logger::errorOnce("Failed to create missing image texture");
	return TextureHolder("textures", std::move(texture));
}

ResourceHolder<sf::Font> ResourceHolders::makeFontHolder() {
	auto font = std::make_unique<sf::Font>();
	if (!font->openFromFile("main/resources/assets/angleshooter/fonts/ubuntu-m.ttf")) Logger::errorOnce("Couldn't find the default font (ubuntu-m.tff)");
	return FontHolder("fonts", std::move(font));
}

ResourceHolder<sf::SoundBuffer> ResourceHolders::makeSoundHolder() {
	auto sound = std::make_unique<sf::SoundBuffer>();
	if (!sound->loadFromFile("main/resources/assets/angleshooter/sounds/empty.ogg")) Logger::errorOnce("Couldn't find the default sound (empty.ogg)");
	return SoundHolder("sounds", std::move(sound));
}

ShaderHolder ResourceHolders::makeShaderHolder() {
	return ShaderHolder("shader");
}