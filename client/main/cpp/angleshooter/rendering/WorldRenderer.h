#pragma once

class WorldRenderer final : public Singleton<WorldRenderer> {
	sf::FloatRect lastViewSize = {{0, 0}, {0, 0}};
	std::unordered_map<int, std::function<void(std::shared_ptr<Entity>, float)>> renderRegistry;
	WorldRenderer();
	
public:
	void render(float deltaTime);
	template<typename T> void registerRenderer(const Identifier& id, std::function<void(std::shared_ptr<T>, float)> renderer);
};