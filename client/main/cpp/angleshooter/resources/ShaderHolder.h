#pragma once
#include <map>
#include <memory>
#include <string>
#include <SFML/Graphics/Shader.hpp>
#include <angleshooter/util/Identifier.h>

class ShaderHolder {
    std::map<int, std::unique_ptr<sf::Shader>> resourceMap;
    std::string directory;

public:
    explicit ShaderHolder(std::string directory);
    void load(const Identifier& id, const Identifier& filename, const Identifier& parameter);
    void load(const Identifier& id, const std::string& filename, const Identifier& parameter);
    void load(const Identifier& id, const Identifier& filename, const std::string& parameter);
    void load(const Identifier& id, const std::string& filename, const std::string& parameter);
    [[nodiscard]] bool has(const Identifier& id) const;
    [[nodiscard]] sf::Shader& get(const Identifier& id);
    [[nodiscard]] sf::Shader* getPointer(const Identifier& id);
};

#include "ShaderHolder.tpp"