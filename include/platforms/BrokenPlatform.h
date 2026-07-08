#pragma once

#include "platforms/Platform.h"

class BrokenPlatform : public Platform {
public:
    BrokenPlatform(sf::Vector2f _position)
        : Platform(_position, SPRITE_PATH) {}
    
private:
    static inline const std::string SPRITE_PATH = "assets/broken_platform.png";
};