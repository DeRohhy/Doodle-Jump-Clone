#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Bullet : public GameObject {
public:
    Bullet(sf::Vector2f _position, sf::Vector2f _direction, float _speed)
        : GameObject(_position, _direction * _speed),
          direction(_direction), speed(_speed) {}

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

    sf::FloatRect getBounds() { return bullet_shape.getGlobalBounds(); }
private:
    static constexpr float BULLET_RADIUS = 6.f;
    sf::CircleShape bullet_shape;
    sf::Vector2f direction;
    float speed;
};