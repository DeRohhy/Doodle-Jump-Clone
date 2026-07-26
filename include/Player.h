#pragma once

#include "GameObject.h"
#include "platforms/Platform.h"

#include <SFML/Graphics.hpp>
#include <string>
#include <optional>

enum PLAYER_STATE {
    RIGHT,
    LEFT,
    SHOOTING,
};

class Player : public GameObject {
public:
    Player(sf::Vector2f _position, sf::Vector2f _velocity,  float _fire_rate)
        : GameObject(_position, _velocity), fire_rate(_fire_rate) {}

    sf::FloatRect getFeetBounds();
    sf::FloatRect getBodyBounds();

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;
    void handleMovement(float delta);
    void handleJump();
    void handleSpringJump();
    void handleShooting();
    void handleScreenWrapping();
    

private:
    bool debug_mode = true;
    static constexpr float FEET_WIDTH_RATIO  = 0.4f;  // 40% of sprite width
    static constexpr float FEET_HEIGHT_RATIO = 0.3f;  // bottom 30% of sprite height
    static constexpr float BODY_HEIGHT_RATIO = 0.7f;  // bottom 30% of sprite height

    const std::string RIGHT_DOODLE_PATH = "assets/right_doodle.png";
    const std::string LEFT_DOODLE_PATH = "assets/left_doodle.png";
    const std::string SHOOTING_POSE_PATH = "assets/shooting_pose.png";
    const std::string NOSE_PATH = "assets/nose.png";

    static constexpr float JUMP_FACTOR = 800.f;
    static constexpr float SPRING_JUMP_FACTOR = 1500.f;
    static constexpr float TOP_SPEED = 700.f;
    static constexpr float VELOCITY_POWER = 0.99f;
    static constexpr float ACCEL_RATE = 7.f;

    sf::Texture right_doodle_texture;
    sf::Texture left_doodle_texture;
    sf::Texture shooting_pose_texture;
    sf::Texture nose_texture;
    std::optional<sf::Sprite> player_sprite, nose_sprite; 

    PLAYER_STATE player_state = PLAYER_STATE::RIGHT;

    // bool fire_rate_timer_timeout = true;
    float fire_rate, fire_rate_timer = 0;
    bool is_shooting = false;
    sf::Vector2f mouse_position;

    void setPlayerState(PLAYER_STATE new_state);

    void drawDebugBounds(sf::RenderWindow& window, sf::FloatRect bounds);
};