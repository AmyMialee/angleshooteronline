#pragma once
#include <angleshooter/resources/Map.h>

class MapRenderer {
	sf::RenderTexture mapTexture;
	sf::Sprite mapSprite;
	
public:
	explicit MapRenderer(Map& map);
	void render(float deltaTime);
};