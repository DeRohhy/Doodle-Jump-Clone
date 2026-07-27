#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"

class Hole : public GameObject {
public:
    Hole(sf::Vector2f _position, bool small_hole);

    void render(sf::RenderWindow& window) override;

    sf::FloatRect getBounds();
    sf::Vector2f getCenterPoint();
private:
    static inline const std::string SPRITE_PATH = "assets/hole.png";
    sf::Texture hole_texture;
    std::optional<sf::Sprite> hole_sprite; 
};