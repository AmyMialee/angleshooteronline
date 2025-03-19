#pragma once

class SoundHolder final : public ResourceHolder<sf::SoundBuffer>, public Singleton<SoundHolder> {
	explicit SoundHolder();
	std::unique_ptr<sf::SoundBuffer>* createDefaultSound();
};