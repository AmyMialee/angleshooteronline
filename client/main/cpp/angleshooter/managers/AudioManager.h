#pragma once

class AudioManager {
	sf::Music music;
	Identifier musicId = Identifier::empty;
	float musicVolume = 1.f;
	float musicPitch = 1.f;
	std::vector<std::tuple<std::shared_ptr<sf::Sound>, float, float>> sounds;
	int tickIndex = 0;
	float scaleVolume(double volume);

public:
	void playMusic(const Identifier& id, float volume = 1.f, float pitch = 1.f);
	void playSound(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector2f position = sf::Vector2f(0.f, 0.f), float attenuation = 1.f);
	void playSound3d(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector3f position = sf::Vector3f(0.f, 0.f, 0.f), float attenuation = 1.f);
	void stopMusic();
	void stopSounds();
	void setMusicVolume(double volume);
	void setSoundVolume(double volume);
	void setMusicPaused(bool paused);
	void tick(float tickDelta);
	[[nodiscard]] sf::Vector2f getListenerPosition2d() const;
	[[nodiscard]] sf::Vector3f getListenerPosition3d() const;
	void setListenerPosition(sf::Vector2f position);
	void setListenerPosition(sf::Vector3f position);
	[[nodiscard]] sf::Vector2f getListenerRotation2d() const;
	[[nodiscard]] sf::Vector3f getListenerRotation3d() const;
	void setListenerRotation(sf::Vector2f direction);
	void setListenerRotation(sf::Vector3f direction);
};