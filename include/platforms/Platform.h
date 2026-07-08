#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <string>


class Platform : public GameObject {
public:
    Platform(sf::Vector2f _position, std::string _platform_path)
        : GameObject(_position, sf::Vector2f()),
          platform_path(_platform_path)
    {}
    
    sf::FloatRect getBounds();

    void start() override;
    void render(sf::RenderWindow& window) override;
private:
    static constexpr float SPRITE_SCALE = 1.1f;
    std::string platform_path;

    sf::Texture platform_texture;
    // SFML 3 removed sf::Sprite’s default constructor
    // so we'll get an error if we say:
    // sf::Sprite player_sprite;
    std::optional<sf::Sprite> platform_sprite; 
};