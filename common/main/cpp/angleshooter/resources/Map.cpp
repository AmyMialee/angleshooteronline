#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "Map.h"

const Identifier Map::DEFAULT_TILE("bricks.png");

Map::Map(const Identifier& mapId, uint16_t rows, uint16_t columns) : mapId(mapId), rows(rows), columns(columns) {
	this->textureGrid.resize(rows);
	this->collisionGrid.resize(rows);
	for (uint16_t row = 0; row < rows; ++row) {
		this->textureGrid[row].resize(columns);
		this->collisionGrid[row].resize(columns);
	}
	for (uint16_t row = 0; row < rows; ++row) {
		for (uint16_t column = 0; column < columns; ++column) {
			this->setTile(row, column, DEFAULT_TILE, row == 0 || column == 0 || row == rows - 1 || column == columns - 1);
		}
	}
}

void Map::setTile(uint16_t row, uint16_t column, const Identifier& texture, bool solid) {
	if (const auto found = this->textureIndices.find(texture.getHash()); found != this->textureIndices.end()) {
		this->textureGrid[row][column] = found->second;
	} else {
		this->textures.push_back(texture);
		const auto textureIndex = static_cast<int>(this->textures.size()) - 1;
		this->textureIndices[texture.getHash()] = textureIndex;
		this->textureGrid[row][column] = textureIndex;
	}
	this->collisionGrid[row][column] = solid;
}

void Map::setSpawnpoint(uint16_t row, uint16_t column, bool add) {
	if (add) {
		this->spawnpoints.emplace_back(row, column);
	} else {
		std::erase(this->spawnpoints, std::make_pair(row, column));
	}
}

const std::vector<std::pair<uint16_t, uint16_t>>& Map::getSpawnpoints() const {
	return this->spawnpoints;
}

sf::Vector2f Map::getRandomSpawnpoint() const {
	if (this->spawnpoints.empty()) return {0, 0};
	auto [first, second] = this->spawnpoints[Util::randomInt(static_cast<int>(this->spawnpoints.size()))];
	return {static_cast<float>(second) * 16.f, static_cast<float>(first) * 16.f};
}

bool Map::isSolid(uint16_t row, uint16_t column) const {
	if (row >= this->rows || column >= this->columns) return true;
	return this->collisionGrid[row][column];
}

const Identifier& Map::getTexture(uint16_t row, uint16_t column) const {
	return this->textures[this->textureGrid[row][column]];
}

uint16_t Map::getRows() const {
	return this->rows;
}

uint16_t Map::getColumns() const {
	return this->columns;
}

const Identifier& Map::getId() const {
	return this->mapId;
}

std::vector<Identifier>& Map::getTextures() {
	return this->textures;
}

std::vector<std::vector<int>>& Map::getTextureGrid() {
	return this->textureGrid;
}

std::vector<std::vector<bool>>& Map::getCollisionGrid() {
	return this->collisionGrid;
}