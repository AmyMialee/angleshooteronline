#pragma once
#include "ShaderHolder.h"  // NOLINT(misc-header-include-cycle)
#include <angleshooter/util/Logger.h>

inline ShaderHolder::ShaderHolder(std::string directory) : directory(std::move(directory)) {}

inline void ShaderHolder::load(const Identifier& id, const Identifier& filename, const Identifier& parameter) {
    this->load(id,
        "main/resources/assets/" + filename.getSpace() + "/" + directory + "/" + filename.getPath(),
        "main/resources/assets/" + parameter.getSpace() + "/" + directory + "/" + parameter.getPath());
}

inline void ShaderHolder::load(const Identifier& id, const std::string& filename, const Identifier& parameter) {
    this->load(id, filename, "main/resources/assets/" + parameter.getSpace() + "/" + directory + "/" + parameter.getPath());
}

inline void ShaderHolder::load(const Identifier& id, const Identifier& filename, const std::string& parameter) {
    this->load(id, "main/resources/assets/" + filename.getSpace() + "/" + directory + "/" + filename.getPath(), parameter);
}

inline void ShaderHolder::load(const Identifier& id, const std::string& filename, const std::string& parameter) {
    if (has(id)) return;
    if (std::unique_ptr<sf::Shader> resource(new sf::Shader()); resource->loadFromFile(filename, parameter)) {
        this->resourceMap.insert(std::make_pair(id.getHash(), std::move(resource)));
    } else {
        Logger::error("ShaderHolder::load - Resource not found: " + id.toString());
    }
}

inline bool ShaderHolder::has(const Identifier& id) const {
    return this->resourceMap.contains(id.getHash());
}

inline sf::Shader& ShaderHolder::get(const Identifier& id) {
    if (const auto found = this->resourceMap.find(id.getHash()); found != this->resourceMap.end()) return *found->second;
    Logger::error("ShaderHolder::get - Tried to load invalid resource: " + id.toString());
    static sf::Shader defaultShader;
    return defaultShader;
}

inline sf::Shader* ShaderHolder::getPointer(const Identifier& id) {
    if (const auto found = this->resourceMap.find(id.getHash()); found != this->resourceMap.end()) return found->second.get();
    Logger::error("ShaderHolder::get - Tried to load invalid resource: " + id.toString());
    return nullptr;
}