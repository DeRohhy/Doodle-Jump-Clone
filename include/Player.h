#pragma once

#include "GameObject.h"
#include "platforms/Platform.h"

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
    void handleJump();
    void handleSpringJump();
    bool isColliding(const sf::FloatRect& collider);
    void handleScreenWrapping();
    

private:
    const std::string RIGHT_DOODLE_PATH = "assets/right_doodle.png";
    const std::string LEFT_DOODLE_PATH = "assets/left_doodle.png";
    static constexpr float JUMP_FACTOR = 800.f;
    static constexpr float SPRING_JUMP_FACTOR = 1500.f;
    static constexpr float TOP_SPEED = 700.f;
    static constexpr float VELOCITY_POWER = 0.99f;
    static constexpr float ACCEL_RATE = 7.f;
    static constexpr float SPRITE_SCALE = 0.75f;

    sf::Texture right_doodle_texture;
    sf::Texture left_doodle_texture;
    std::optional<sf::Sprite> player_sprite; 

    Direction facing_direction = Direction::RIGHT;

    void setDirection(Direction new_direction);
};