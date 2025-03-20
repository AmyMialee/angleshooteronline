#pragma once

class ShaderHolder final : public InstancedSingleton<ShaderHolder> {
	friend class InstancedSingleton<ShaderHolder>;
    std::map<int, std::unique_ptr<sf::Shader>> resourceMap;
    ShaderHolder() = default;

public:
	static ShaderHolder& get();
    void load(const Identifier& id, const Identifier& filename, const Identifier& parameter);
    void load(const Identifier& id, const std::string& filename, const Identifier& parameter);
    void load(const Identifier& id, const Identifier& filename, const std::string& parameter);
    void load(const Identifier& id, const std::string& filename, const std::string& parameter);
    [[nodiscard]] bool has(const Identifier& id) const;
    [[nodiscard]] sf::Shader& get(const Identifier& id);
    [[nodiscard]] sf::Shader* getPointer(const Identifier& id);
};

#include "ShaderHolder.tpp"