#pragma once

class World {
	Map map;
	std::unordered_map<uint16_t, std::shared_ptr<Entity>> gameObjects;
	uint16_t nextId = 0;

public:
	World();
	virtual ~World() = default;
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	void tick(float deltaTime);
	void init();
	std::shared_ptr<Entity> addGameObject(const std::function<Entity(uint16_t)>& createEntity);
	[[nodiscard]] std::vector<std::shared_ptr<Entity>> getGameObjects();
	[[nodiscard]] Map& getMap();
	
	virtual void playMusic(const Identifier& id, float volume = 1.f, float pitch = 1.f) = 0;
	virtual void playSound(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector2f position = sf::Vector2f(0.f, 0.f), float attenuation = 1.f) = 0;
	virtual void playSound3d(const Identifier& id, float volume = 1.f, float pitch = 1.f, sf::Vector3f position = sf::Vector3f(0.f, 0.f, 0.f), float attenuation = 1.f) = 0;
};