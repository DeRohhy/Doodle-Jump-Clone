#pragma once

#include "platforms/Platform.h"

class MovingPlatform : public Platform {
public:
    MovingPlatform(sf::Vector2f _position, float _speed)
        : Platform(_position, SPRITE_PATH),
          speed(_speed) {}

    void update(float delta) override;

private:
    float half_movement_range;
    float speed;
    static inline const std::string SPRITE_PATH = "assets/moving_platform.png";
};