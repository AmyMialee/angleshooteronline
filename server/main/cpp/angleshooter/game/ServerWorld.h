#pragma once

class ServerWorld final : public World {
protected:
	ServerWorld() = default;
	~ServerWorld() override = default;

public:
	ServerWorld(const ServerWorld&) = delete;
	void operator=(const ServerWorld&) = delete;

	std::shared_ptr<PlayerEntity> spawnPlayer(ClientConnection& sender);
	std::shared_ptr<BulletEntity> spawnBullet(int color, sf::Vector2f position, sf::Vector2f velocity);

	void playMusic(const Identifier& id, float volume, float pitch) override;
	void playSound(const Identifier& id, float volume, float pitch, sf::Vector2f position, float attenuation) override;
	void playSound3d(const Identifier& id, float volume, float pitch, sf::Vector3f position, float attenuation) override;
	void loadMap(const Identifier& id) override;

	static ServerWorld& get() {
		static ServerWorld instance;
		return instance;
	}
};