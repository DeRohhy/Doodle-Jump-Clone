#pragma once

#include "enemies/Enemy.h"

#include <string>

class StationaryEnemy : public Enemy {
public:
    StationaryEnemy(sf::Vector2f _position, int _health) 
        : Enemy(_position, SPRITE_PATH, _health){}

private:
    static inline const std::string SPRITE_PATH = "assets/enemies/stationary_enemy.png";
};