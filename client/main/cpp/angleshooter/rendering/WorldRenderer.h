#pragma once

class WorldRenderer final {
	sf::FloatRect lastViewSize = {{0, 0}, {0, 0}};
	std::unordered_map<int, std::function<void(std::shared_ptr<Entity>, float)>> renderRegistry;

protected:
	WorldRenderer();
	~WorldRenderer() = default;
	
public:
	WorldRenderer(const WorldRenderer&) = delete;
	void operator=(const WorldRenderer&) = delete;
	void render(float deltaTime);
	template<typename T> void registerRenderer(const Identifier& id, std::function<void(std::shared_ptr<T>, float)> renderer);

	static WorldRenderer& get() {
		static WorldRenderer instance;
		return instance;
	}
};