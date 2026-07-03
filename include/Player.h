#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

class Player : public GameObject {
public:
    Player(sf::Vector2f _position, sf::Vector2f _velocity)
        : GameObject(_position, _velocity) {}
    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    const std::string RIGHT_DOODLE_PATH = "assets/right_doodle.png";
    const std::string LEFT_DOODLE_PATH = "assets/left_doodle.png";

    sf::Texture left_doodle_texture;
    sf::Texture right_doodle_texture;
    // SFML 3 removed sf::Sprite’s default constructor
    // so we'll get an error if we say:
    // sf::Sprite player_sprite;
    std::optional<sf::Sprite> player_sprite; 
};