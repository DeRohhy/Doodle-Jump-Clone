#pragma once

#include "singletons/Singleton.h"

#include <unordered_map>
#include <stdexcept>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

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

template<>
inline sf::Font& ResourceManager<sf::Font>::get(const std::string& file_path) {
    if (resources.find(file_path) == resources.end()) {
        sf::Font new_resource;
        if (!new_resource.openFromFile(file_path)) {
            throw std::runtime_error("ResourceManager: Couldn't load font: " + file_path);
        }
        resources[file_path] = std::move(new_resource);
    }
    return resources[file_path];
}

template<>
inline sf::Music& ResourceManager<sf::Music>::get(const std::string& file_path) {
    if (resources.find(file_path) == resources.end()) {
        sf::Music& new_resource = resources[file_path]; 
        
        if (!new_resource.openFromFile(file_path)) {
            resources.erase(file_path); // Clean up if it fails
            throw std::runtime_error("ResourceManager: Couldn't load music: " + file_path);
        }
    }
    return resources[file_path];
}
