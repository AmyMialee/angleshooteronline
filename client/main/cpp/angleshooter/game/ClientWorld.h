#pragma once

class ClientWorld final : public World {
protected:
	ClientWorld();
	~ClientWorld() override = default;

public:
	MapRenderer* mapRenderer;
	ClientWorld(const ClientWorld&) = delete;
	void operator=(const ClientWorld&) = delete;

	void spawnPlayer(const std::string& name, int color, sf::Vector2f position, bool isClientPlayer);

	void playMusic(const Identifier& id, float volume, float pitch) override;
	void playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) override;
	void playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) override;
	void loadMap(const Identifier& id) override;

	static ClientWorld& get() {
		static ClientWorld instance;
		return instance;
	}
};