#include "PreCompiledClient.h"
#include "SoundHolder.h"

SoundHolder::SoundHolder() : ResourceHolder("sounds", std::move(*createDefaultSound())) {}

std::unique_ptr<sf::SoundBuffer>* SoundHolder::createDefaultSound() {
	auto sound = std::make_unique<sf::SoundBuffer>();
	if (!sound->loadFromFile("main/resources/assets/angleshooter/sounds/empty.ogg")) Logger::errorOnce("Couldn't find the default sound (empty.ogg)");
	return &sound;
}