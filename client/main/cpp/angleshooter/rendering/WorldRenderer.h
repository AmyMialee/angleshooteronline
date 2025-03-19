#pragma once

class WorldRenderer {
	sf::FloatRect lastViewSize = {{0, 0}, {0, 0}};
	
public:
	WorldRenderer() = default;
	WorldRenderer(const WorldRenderer&) = delete;
	WorldRenderer& operator=(const WorldRenderer&) = delete;
	void render(float deltaTime);
};