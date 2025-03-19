#pragma once

class MapRenderer {
	sf::RenderTexture mapTexture;
	sf::Sprite mapSprite;
	
public:
	explicit MapRenderer(Map& map);
	void render(float deltaTime);
};