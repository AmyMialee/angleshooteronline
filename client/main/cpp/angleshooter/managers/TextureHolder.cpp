#include "PreCompiledClient.h"
#include "TextureHolder.h"

TextureHolder::TextureHolder() : ResourceHolder("textures", std::move(createDefaultTexture())) {}

std::unique_ptr<sf::Texture> TextureHolder::createDefaultTexture() {
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
	return texture;
}