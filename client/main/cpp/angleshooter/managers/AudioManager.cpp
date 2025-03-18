#include "PreCompiledClient.h"
#include "AudioManager.h"

/**
 * Input takes the form of a double between 0 and 1, and is scaled to the power of 4, and brought to the range 0 to 100.
 * Power of 4 was chosen based on this article: https://www.dr-lex.be/info-stuff/volumecontrols.html#table1
 */
float AudioManager::scaleVolume(double volume) {
	if (static_cast<int>(volume * 100) == 0) return 0;
	return static_cast<float>(std::clamp(std::pow(volume, 4) * 100, 0., 100.));
}

void AudioManager::playMusic(const Identifier& id, float volume, float pitch) {
	if (this->musicId.getHash() == id.getHash()) {
		this->musicVolume = volume;
		this->musicPitch = pitch;
		this->music.setPitch(pitch);
		setMusicVolume(volume * ClientContext::get()->getOptionsManager()->getMasterVolume() * ClientContext::get()->getOptionsManager()->getMusicVolume());
		return;
	}
	if (this->music.openFromFile("main/resources/assets/" + id.getSpace() + "/music/" + id.getPath())) {
		this->musicId = id;
		this->music.setLooping(true);
		this->musicVolume = volume;
		this->musicPitch = pitch;
		this->music.setPitch(pitch);
		setMusicVolume(volume * ClientContext::get()->getOptionsManager()->getMasterVolume() * ClientContext::get()->getOptionsManager()->getMusicVolume());
		this->music.play();
	} else {
		this->musicId = Identifier::empty;
		this->music.stop();
		Logger::warn("MusicManager::playMusic - Music not found: " + id.toString());
	}
}

void AudioManager::playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) {
	this->playSound3d(id, volume, pitch, sf::Vector3f(position.x, 0.f, position.y), attenuation);
}

void AudioManager::playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) {
	const auto buffer = ClientContext::get()->getSoundHolder()->getPointer(id);
	auto sound = std::make_shared<sf::Sound>(*buffer);
	auto tuple = std::tuple(sound, volume, pitch);
	sound->setVolume(scaleVolume(volume * ClientContext::get()->getOptionsManager()->getMasterVolume() * ClientContext::get()->getOptionsManager()->getSoundVolume()));
	sound->setPitch(pitch);
	sound->setPosition(position);
	sound->setAttenuation(attenuation);
	sound->play();
	this->sounds.emplace_back(tuple);
}

void AudioManager::stopMusic() {
	this->music.stop();
}

void AudioManager::stopSounds() {
	for (auto& [sound, volume, pitch] : this->sounds) sound->stop();
	this->sounds.clear();
}

void AudioManager::setMusicVolume(double volume) {
	this->music.setVolume(scaleVolume(volume * musicVolume));
}

void AudioManager::setSoundVolume(double volume) {
	for (auto& [sound, soundVolume, pitch] : this->sounds) {
		sound->setVolume(scaleVolume(volume * soundVolume));
	}
}

void AudioManager::setMusicPaused(bool paused) {
	if (paused) {
		this->music.pause();
	} else {
		this->music.play();
	}
}

void AudioManager::tick(float) {
	this->tickIndex++;
	std::erase_if(this->sounds, [](const std::tuple<std::shared_ptr<sf::Sound>, float, float>& tuple) {
		const auto& sound = std::get<0>(tuple);
		return sound->getStatus() != sf::Sound::Status::Playing;
	});
}

sf::Vector2f AudioManager::getListenerPosition2d() const {
	auto position = sf::Listener::getPosition();
	return {position.x, position.z};
}

sf::Vector3f AudioManager::getListenerPosition3d() const {
	return sf::Listener::getPosition();
}

void AudioManager::setListenerPosition(sf::Vector2f position) {
	sf::Listener::setPosition({position.x, 0.f, position.y});
}

void AudioManager::setListenerPosition(sf::Vector3f position) {
	sf::Listener::setPosition(position);
}

sf::Vector2f AudioManager::getListenerRotation2d() const {
	auto direction = sf::Listener::getDirection();
	return {direction.x, direction.z};
}

sf::Vector3f AudioManager::getListenerRotation3d() const {
	return sf::Listener::getDirection();
}

void AudioManager::setListenerRotation(sf::Vector2f direction) {
	sf::Listener::setDirection({direction.x, 0.f, direction.y});
}

void AudioManager::setListenerRotation(sf::Vector3f direction) {
	sf::Listener::setDirection(direction);
}