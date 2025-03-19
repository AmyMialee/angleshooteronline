#pragma once

class ServerWorld final : public World {
public:
	void playMusic(const Identifier& id, float volume, float pitch) override;
	void playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) override;
	void playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) override;
};