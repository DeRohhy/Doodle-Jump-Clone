#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <string>

class Enemy : public GameObject {
public:
    Enemy(sf::Vector2f _position, std::string _enemy_path, int _health)
        : GameObject(_position, sf::Vector2f()),
          enemy_path(_enemy_path),
          health(_health)
    {}
    
    sf::FloatRect getBounds();

    void start() override;
    virtual void render(sf::RenderWindow& window) override;

    void decrementHealth() { health--; }
    bool isDead() { return health <= 0; }

protected:
    static constexpr float VERTICAL_SPEED = 15.f;
    
    std::string enemy_path;
    sf::Texture enemy_texture;
    std::optional<sf::Sprite> enemy_sprite;

    int health;
};