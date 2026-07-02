#pragma once

#include "singletons/Singleton.h"

#include <unordered_map>
#include <stdexcept>
#include <string>

template<typename type>
class ResourceManager : public Singleton<ResourceManager<type>> {
public:
    type& get(const std::string& file_path);
private:
    std::unordered_map<std::string, type> resources;
};

template<typename type>
type& ResourceManager<type>::get(const std::string& file_path) {
    if (resources.find(file_path) == resources.end()) {
        type new_resource;
        if (!new_resource.loadFromFile(file_path)) {
            throw std::runtime_error("ResourceManager: Couldn't load resource: " + file_path);
        }
        resources[file_path] = std::move(new_resource);
    }
    return resources[file_path];
}