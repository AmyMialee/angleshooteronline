#pragma once

class SoundHolder final : public ResourceHolder<sf::SoundBuffer>, public InstancedSingleton<SoundHolder> {
	friend class InstancedSingleton;
	explicit SoundHolder();
	std::unique_ptr<sf::SoundBuffer> createDefaultSound();
};