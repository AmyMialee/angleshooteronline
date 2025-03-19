#include "PreCompiledClient.h"
#include "FontHolder.h"

FontHolder::FontHolder() : ResourceHolder("fonts", std::move(*createDefaultFont())) {}

std::unique_ptr<sf::Font>* FontHolder::createDefaultFont() {
	auto font = std::make_unique<sf::Font>();
	if (!font->openFromFile("main/resources/assets/angleshooter/fonts/ubuntu-m.ttf")) Logger::errorOnce("Couldn't find the default font (ubuntu-m.tff)");
	return &font;
}