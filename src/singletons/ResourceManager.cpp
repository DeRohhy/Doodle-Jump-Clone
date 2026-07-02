#include "singletons/ResourceManager.h"

#include <stdexcept>

template<typename type>
type& ResourceManager<type>::get(const std::string file_path) {
    auto it = resources.find(file_path);

    if (it != resources.end()) {
        return it->value();
    }

    // Othersie, load it from file
    type new_resource;
    if (!new_resource.loadFromFile(file_path)) {
        throw std::runtime_error("Error: Resource not found: %s", file_path);
    }

    resources[file_path] = new_resource;

    return new_resource;
}