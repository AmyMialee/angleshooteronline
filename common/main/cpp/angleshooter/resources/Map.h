#pragma once

class Map {
	Identifier mapId;
	std::vector<Identifier> textures;
	std::unordered_map<int, int> textureIndices;
	std::vector<std::vector<int>> textureGrid;
	std::vector<std::vector<bool>> collisionGrid;
	uint16_t rows;
	uint16_t columns;
	std::vector<std::pair<uint16_t, uint16_t>> spawnpoints;

public:
	static const Identifier DEFAULT_TILE;
	Map(const Identifier& mapId, uint16_t rows, uint16_t columns);
	[[nodiscard]] bool isSolid(uint16_t row, uint16_t column) const;
	[[nodiscard]] const Identifier& getTexture(uint16_t row, uint16_t column) const;
	void setTile(uint16_t row, uint16_t column, const Identifier& texture, bool solid);
	void setSpawnpoint(uint16_t row, uint16_t column, bool add = true);
	[[nodiscard]] const std::vector<std::pair<uint16_t, uint16_t>>& getSpawnpoints() const;
	[[nodiscard]] sf::Vector2f getRandomSpawnpoint() const;
	[[nodiscard]] uint16_t getRows() const;
	[[nodiscard]] uint16_t getColumns() const;
	[[nodiscard]] const Identifier& getId() const;
	[[nodiscard]] std::vector<Identifier>& getTextures();
	[[nodiscard]] std::vector<std::vector<int>>& getTextureGrid();
	[[nodiscard]] std::vector<std::vector<bool>>& getCollisionGrid();
};