#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>
#include <angleshooter/util/Identifier.h>

template<typename T>
class ResourceHolder {
    std::set<int> attemptedResources;
    std::map<int, std::unique_ptr<T>> resourceMap;
    std::string directory;
    std::unique_ptr<T> defaultResource;

public:
    explicit ResourceHolder(std::string directory, std::unique_ptr<T> defaultResource);
    void load(const Identifier& id);
    void load(const Identifier& id, const std::string& filename);
    template<typename P> void load(Identifier id, const std::string& filename, const P& parameter);
    [[nodiscard]] bool has(const Identifier& id) const;
    [[nodiscard]] T& get(const Identifier& id);
    [[nodiscard]] T* getPointer(const Identifier& id);
    [[nodiscard]] const T& getDefault() const;
};

#include "ResourceHolder.tpp"