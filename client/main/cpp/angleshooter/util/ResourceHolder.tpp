#pragma once
#include <angleshooter/util/Logger.h>
#include "ResourceHolder.h"  // NOLINT(misc-header-include-cycle)

template <typename T>
ResourceHolder<T>::ResourceHolder(std::string directory, std::unique_ptr<T> defaultResource) : directory(std::move(directory)), defaultResource(std::move(defaultResource)) {}

template<typename T>
void ResourceHolder<T>::load(const Identifier& id) {
    if (has(id)) return;
    this->load(id, "main/resources/assets/" + id.getSpace() + "/" + directory + "/" + id.getPath());
}

template<typename T>
void ResourceHolder<T>::load(const Identifier& id, const std::string& filename) {
    if (has(id)) return;
    if (std::unique_ptr<T> resource(new T()); resource->loadFromFile(filename)) {
        this->resourceMap.insert(std::make_pair(id.getHash(), std::move(resource)));
    } else {
        this->resourceMap.insert(std::make_pair(id.getHash(), std::move(this->defaultResource.get())));
        Logger::warn("ResourceHolder::load - " + this->directory + " Resource not found: " + id.toString());
    }
}

template<typename T>
template<typename P>
void ResourceHolder<T>::load(Identifier id, const std::string& filename, const P& parameter) {
    if (has(id)) return;
    if (std::unique_ptr<T> resource(new T()); resource->loadFromFile(filename, parameter)) {
        this->resourceMap.insert(std::make_pair(id.getHash(), std::move(resource)));
    } else {
        this->resourceMap.insert(std::make_pair(id.getHash(), std::move(this->defaultResource.get())));
        Logger::warn("ResourceHolder::load with parameter - " + this->directory + " Resource not found: " + id.toString());
    }
}

template <typename T>
bool ResourceHolder<T>::has(const Identifier& id) const {
    return this->resourceMap.contains(id.getHash());
}

template<typename T>
T& ResourceHolder<T>::get(const Identifier& id) {
    auto found = this->resourceMap.find(id.getHash());
    if (found != this->resourceMap.end()) return *found->second;
    if (!attemptedResources.contains(id.getHash())) {
        attemptedResources.insert(id.getHash());
        Logger::debug("ResourceHolder::get - Late Loading " + this->directory + " Resource: " + id.toString());
        this->load(id);
        auto retry = this->resourceMap.find(id.getHash());
        if (retry != this->resourceMap.end()) return *retry->second;
    }
    return *this->defaultResource;
}

template<typename T>
T* ResourceHolder<T>::getPointer(const Identifier& id) {
    auto found = this->resourceMap.find(id.getHash());
    if (found != this->resourceMap.end()) return found->second.get();
    if (!attemptedResources.contains(id.getHash())) {
        attemptedResources.insert(id.getHash());
        Logger::debug("ResourceHolder::get - Late Loading " + this->directory + " Resource: " + id.toString());
        this->load(id);
        auto retry = this->resourceMap.find(id.getHash());
        if (retry != this->resourceMap.end()) return retry->second.get();
    }
    return this->defaultResource.get();
}

template <typename T>
const T& ResourceHolder<T>::getDefault() const {
    return *this->defaultResource;
}