#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "World.h"

World::World() : map(MapLoader::loadMap(Identifier::fromString("testmaplarge"))) {}

void World::init() {
	this->gameObjects.clear();
	// const auto player1 = std::make_shared<PlayerEntity>("player");
	// player1->setPosition(this->map.getRandomSpawnpoint());
	// addGameObject(player1);
	this->playerData.clear();
	this->playerData.emplace("player", PlayerData());
}

void World::tick(float deltaTime) {
	std::vector<GameObject::Pointer> objectList;
	objectList.reserve(this->gameObjects.size());
	for (const auto& value : this->gameObjects | std::views::values) objectList.push_back(value);
	for (auto main = objectList.begin(); main != objectList.end(); ++main) main->get()->baseTick(deltaTime);
	std::vector<GameObject::Pointer> objects;
	objects.reserve(this->gameObjects.size());
	for (const auto& value : this->gameObjects | std::views::values) objects.push_back(value);
	std::vector<GameObject::Pair> pairs;
	for (auto main = objects.begin(); main != objects.end(); ++main) {
		for (auto sub = main + 1; sub != objects.end(); ++sub) {
			pairs.emplace_back(main->get(), sub->get());
		}
	}
	for (const auto& [first, second] : pairs) {
		if (first->isColliding(*second)) {
			first->onCollision(*second);
			second->onCollision(*first);
		}
	}
	auto iterator = this->gameObjects.begin();
	while (iterator != this->gameObjects.end()) {
		auto a = (*iterator).second;
		if (iterator->second->isMarkedForRemoval()) {
			iterator = this->gameObjects.erase(iterator);
		} else {
			++iterator;
		}
	}
}

void World::addGameObject(std::shared_ptr<GameObject> gameObject) {
	auto id = ++this->nextId;
	gameObject->setId(id);
	this->gameObjects.emplace(id, std::move(gameObject));
}

std::vector<std::shared_ptr<GameObject>> World::getGameObjects() {
	std::vector<std::shared_ptr<GameObject>> values;
	values.reserve(this->gameObjects.size());
	for (const auto& value : this->gameObjects | std::views::values) values.push_back(value);
	return values;
}

Map& World::getMap() {
	return this->map;
}

PlayerData* World::getPlayerData(const std::string& id) {
	const auto it = this->playerData.find(id);
	if (it == this->playerData.end()) return nullptr;
	return &it->second;
}

std::unordered_map<std::string, PlayerData> World::getPlayerData() {
	return this->playerData;
}
