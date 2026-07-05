#pragma once

#include "GameObject.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

enum Direction {
    RIGHT,
    LEFT
};

class Player : public GameObject {
public:
    Player(sf::Vector2f _position, sf::Vector2f _velocity)
        : GameObject(_position, _velocity) {}

    sf::FloatRect getBounds();

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;
    void handleMovement(float delta);

private:
    const std::string RIGHT_DOODLE_PATH = "assets/right_doodle.png";
    static constexpr float JUMP_FACTOR = 800.f;
    static constexpr float TOP_SPEED = 700.f;
    static constexpr float VELOCITY_POWER = 0.99f;
    static constexpr float ACCEL_RATE = 7.f;

    sf::Texture right_doodle_texture;
    // SFML 3 removed sf::Sprite’s default constructor
    // so we'll get an error if we say:
    // sf::Sprite player_sprite;
    std::optional<sf::Sprite> player_sprite; 

    Direction facing_direction = Direction::RIGHT;

    void setDirection(Direction new_direction);
};