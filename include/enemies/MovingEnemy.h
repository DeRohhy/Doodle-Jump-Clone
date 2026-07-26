#pragma once

#include "enemies/Enemy.h"

#include <string>

class MovingEnemy : public Enemy {
public:
    MovingEnemy(sf::Vector2f _position, int _horizontal_speed, int _health) 
        : Enemy(_position, SPRITE_PATH, _health), horizontal_speed(_horizontal_speed) {}

    void update(float delta) override;
private:
    static inline const std::string SPRITE_PATH = "assets/enemies/moving_enemy.png";
    static constexpr float SIDE_MARGIN = 50.f;

    float horizontal_speed;
};