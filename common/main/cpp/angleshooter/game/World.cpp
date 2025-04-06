#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "World.h"

World::World() : map(Map(Identifier("empty"), 1, 1)) {}

void World::init() {
	this->gameObjects.clear();
}

void World::tick(float deltaTime) {
	this->age++;
	std::vector<std::shared_ptr<Entity>> objectList;
	objectList.reserve(this->gameObjects.size());
	for (const auto& value : this->gameObjects | std::views::values) objectList.push_back(value);
	for (auto main = objectList.begin(); main != objectList.end(); ++main) main->get()->tick(deltaTime);
}

std::shared_ptr<Entity> World::spawnEntity(std::shared_ptr<Entity> entity) {
	this->gameObjects.emplace(entity->getId(), std::move(entity));
	return entity;
}

std::vector<std::shared_ptr<Entity>> World::getEntities() {
	std::vector<std::shared_ptr<Entity>> values;
	values.reserve(this->gameObjects.size());
	for (const auto& value : this->gameObjects | std::views::values) values.push_back(value);
	return values;
}

Map* World::getMap() {
	return &this->map;
}

uint16_t World::getNextId() {
	return ++this->nextId;
}

int World::getAge() const {
	return this->age;
}

void World::loadMap(const Identifier& id) {
	this->map = MapLoader::loadMap(id);
}
