#pragma once

#include "singletons/Singleton.h"

#include <unordered_map>
#include <string>

template<typename type>
class ResourceManager : public Singleton<ResourceManager<type>> {
public:
    type& get(const std::string& file_path);
private:
    std::unordered_map<std::string, type> resources;
};