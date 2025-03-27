#include "main/cpp/angleshooter/PreCompiledHeaders.h"
#include "MapLoader.h"

Map MapLoader::loadMap(const Identifier& id) {
	try {
		const auto fileName = "main/resources/data/" + id.getSpace() + "/maps/" + id.getPath() + ".json";
		if (std::ifstream file(fileName); file.is_open()) {
			nlohmann::json json;
			file >> json;
			file.close();
			auto textureMap = std::map<char, Identifier>();
			for (auto textures = json.value("textures", nlohmann::json::object()); const auto& [key, value]: textures.items()) {
				if (key.length() != 1) throw std::runtime_error("Invalid texture key '" + key + "'");
				textureMap[key[0]] = Identifier::fromString(value.get<std::string>());
			}
			auto textureGrid = json.value("textureGrid", nlohmann::json::array());
			auto collision = json.value("collision", nlohmann::json::array());
			auto spawnpoints = json.value("spawnpoints", nlohmann::json::array());
			if (textureGrid.size() != collision.size() || textureGrid.size() != spawnpoints.size()) throw std::runtime_error("Inconsistent grid row number");
			auto width = -1;
			for (auto i = 0; i < static_cast<int>(textureGrid.size()); ++i) {
				const auto& textureRow = textureGrid[i];
				const auto& collisionRow = collision[i];
				const auto& spawnpointRow = spawnpoints[i];
				auto textureWidth = static_cast<int>(textureRow.get<std::string>().length());
				auto collisionWidth = static_cast<int>(collisionRow.get<std::string>().length());
				if (auto spawnpointWidth = static_cast<int>(spawnpointRow.get<std::string>().length()); textureWidth != collisionWidth || textureWidth != spawnpointWidth) throw std::runtime_error("Inconsistent row width across grids");
				if (width == -1) {
					width = textureWidth;
				} else if (width != textureWidth) {
					throw std::runtime_error("Inconsistent row width across rows");
				}
			}
			auto map = Map(id, static_cast<uint16_t>(textureGrid.size()), static_cast<uint16_t>(width));
			for (uint16_t row = 0; row < map.getRows(); ++row) {
				const auto& textureRowString = textureGrid[row].get<std::string>();
				const auto& collisionRowString = collision[row].get<std::string>();
				for (uint16_t column = 0; column < map.getColumns(); ++column) {
					if (!textureMap.contains(textureRowString[column])) {
						auto buf = std::stringstream();
						buf << "Invalid texture key '" << textureRowString[column] << "'";
						throw std::runtime_error(buf.str());
					}
					const auto& texture = textureMap[textureRowString[column]];
					map.setTile(row, column, texture, collisionRowString[column] != ' ');
				}
				const auto& spawnpointsRowString = spawnpoints[row].get<std::string>();
				for (uint16_t column = 0; column < map.getColumns(); ++column) {
					map.setSpawnpoint(row, column, spawnpointsRowString[column] != ' ');
				}
			}
			return map;
		}
		throw std::runtime_error("Couldn't open file");
	} catch (const std::exception& e) {
		Logger::error("Failed to load map \"" + id.toString() + "\": " + e.what());
	}
	static auto missingId = Identifier("missing");
	auto map = Map(missingId, 3, 3);
	return map;
}