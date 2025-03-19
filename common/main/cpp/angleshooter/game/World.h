#pragma once

class World final {
	Map map;
	std::unordered_map<uint32_t, std::shared_ptr<GameObject>> gameObjects;
	std::unordered_map<std::string, PlayerData> playerData;
	uint32_t nextId = 0;

public:
	World();
	World(const World&) = delete;
	World& operator=(const World&) = delete;
	void tick(float deltaTime);
	void init();
	void addGameObject(std::shared_ptr<GameObject> gameObject);
	[[nodiscard]] std::vector<std::shared_ptr<GameObject>> getGameObjects();
	[[nodiscard]] Map& getMap();
	[[nodiscard]] PlayerData* getPlayerData(const std::string& id);
	[[nodiscard]] std::unordered_map<std::string, PlayerData> getPlayerData();
};