#pragma once
#include "Map.h"

class MapLoader {
public:
	static Map loadMap(const Identifier& id);
};