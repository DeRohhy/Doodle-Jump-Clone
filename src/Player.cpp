#include "Player.h"

#include "singletons/ResourceManager.h"
#include "GameConstants.h"
#include "Theme.h"
#include "platforms/BrokenPlatform.h"


#include <cmath>
#include <numbers>
#include <iostream>

sf::FloatRect Player::getFeetBounds() {
    if (!player_sprite) {
        return sf::FloatRect();
    }
    const sf::FloatRect global_bounds = player_sprite->getGlobalBounds();

    const sf::Vector2f feet_size = {global_bounds.size.x * FEET_WIDTH_RATIO,
                                    global_bounds.size.y * FEET_HEIGHT_RATIO};

    const sf::Vector2f feet_position = {
        global_bounds.position.x + (global_bounds.size.x - feet_size.x) / 2.f,
        global_bounds.position.y + (global_bounds.size.y - feet_size.y)};

    return sf::FloatRect(feet_position, feet_size);
}

sf::FloatRect Player::getBodyBounds() {
    if (!player_sprite) {
        return sf::FloatRect();
    }
    const sf::FloatRect global_bounds = player_sprite->getGlobalBounds();

    const sf::Vector2f body_size = {global_bounds.size.x * FEET_WIDTH_RATIO,
                                    global_bounds.size.y * (BODY_HEIGHT_RATIO - FEET_HEIGHT_RATIO)};

    const sf::Vector2f body_position = {
        global_bounds.position.x + (global_bounds.size.x - body_size.x) / 2.f,
        global_bounds.position.y + (global_bounds.size.y - (body_size.y + getFeetBounds().size.y))};

    return sf::FloatRect(body_position, body_size);
}

void Player::start() {
    right_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(RIGHT_DOODLE_PATH);
    left_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(LEFT_DOODLE_PATH);
    shooting_pose_texture = ResourceManager<sf::Texture>::getInstance().get(SHOOTING_POSE_PATH);
    nose_texture = ResourceManager<sf::Texture>::getInstance().get(NOSE_PATH);

    player_sprite.emplace(right_doodle_texture); // construct in-place with texture
    nose_sprite.emplace(nose_texture);

    // offset player sprite so that
    // the bottom of the feet is at local y = 0
    // and the center between player legs is at local x = 0
    sf::Vector2f player_local_bounds = player_sprite->getLocalBounds().size;
    player_sprite->setOrigin({player_local_bounds.x / 2, player_local_bounds.y});
    player_sprite->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});

    sf::Vector2f nose_local_bounds = nose_sprite->getLocalBounds().size;
    nose_sprite->setOrigin({nose_local_bounds.x / 2, nose_local_bounds.y / 2});
    nose_sprite->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});

    fire_rate_timer = fire_rate;
}

void Player::update(float delta) {
    handleScreenWrapping();
    handleMovement(delta);
    handleShooting();
    velocity.y += GameConstants::GRAVITY * delta;
    position += velocity * delta;

    if (player_sprite) player_sprite->setPosition(position);
    if (nose_sprite) {
        nose_sprite->setPosition({
            position.x,
            position.y - player_sprite->getGlobalBounds().size.y / 2
        });
    }

    fire_rate_timer -= delta;
    if (fire_rate_timer <= 0) {
        fire_rate_timer = 0;
        is_shooting = false;
    }
}

void Player::render(sf::RenderWindow& window) {
    // update mouse position relative e to the window/view
    mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    if (player_sprite) window.draw(*player_sprite);
    if (is_shooting && nose_sprite) window.draw(*nose_sprite);

    if (debug_mode) drawDebugBounds(window, getFullBodyBounds());
}

void Player::handleMovement(float delta) {
    static int last_valid_direction = 1;
    int direction = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction = -1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction = 1;

    if (direction != 0)
        last_valid_direction = direction;
    
    if (!is_shooting) {
        setPlayerState(last_valid_direction == -1 ? PLAYER_STATE::LEFT : PLAYER_STATE::RIGHT);
    }

    float target_speed = static_cast<float>(direction) * TOP_SPEED;
    float speed_difference = target_speed - velocity.x;
    float delta_velocity = std::pow(std::abs(speed_difference) * ACCEL_RATE, VELOCITY_POWER) * (speed_difference > 0 ? 1 : -1);

    velocity.x += delta_velocity * delta;
}

void Player::handleShooting() {
    if (!is_shooting && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
        // reset fire rate timer
        fire_rate_timer = fire_rate;
        is_shooting = true;



        float half_width = GameConstants::SCREEN_WIDTH / 2;
        float relative_distance = mouse_position.x - half_width;


        if (relative_distance < -half_width)
            relative_distance = -half_width;
        else if (relative_distance > half_width)
            relative_distance = half_width;

        static constexpr float MAX_NOSE_ANGLE_DEG = 30.f;
        float nose_angle_degree = MAX_NOSE_ANGLE_DEG * (relative_distance / half_width);

        if (nose_sprite) {
            nose_sprite->setRotation(sf::degrees(nose_angle_degree ));
        }

        setPlayerState(PLAYER_STATE::SHOOTING);
    }
}

void Player::handleJump() {
    velocity.y = -JUMP_FACTOR;
}

void Player::handleSpringJump() {
    velocity.y = -SPRING_JUMP_FACTOR;
}

void Player::setPlayerState(PLAYER_STATE new_state) {
    if (player_state == new_state) return; 
        
    player_state = new_state;

    if (!player_sprite) return;

    if (new_state == PLAYER_STATE::LEFT) {
        player_sprite->setTexture(left_doodle_texture);
    } else if (new_state == PLAYER_STATE::RIGHT) {
        player_sprite->setTexture(right_doodle_texture);
    } else if (new_state == PLAYER_STATE::SHOOTING) {
        player_sprite->setTexture(shooting_pose_texture);
    }
}

void Player::handleScreenWrapping() {
    static constexpr float left_margin = 0.f;
    static constexpr float right_margin = 0.f;
    sf::Vector2f center_of_feet_pos = position;

    if (center_of_feet_pos.x < left_margin) {
        position.x = GameConstants::SCREEN_WIDTH;
    } else if (center_of_feet_pos.x > GameConstants::SCREEN_WIDTH + right_margin) {
        position.x = left_margin;
    } 
}

void Player::drawDebugBounds(sf::RenderWindow& window, sf::FloatRect bounds) {
    sf::RectangleShape box;

    box.setPosition(bounds.position);
    box.setSize(bounds.size);

    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(sf::Color::Red);
    box.setOutlineThickness(5.f);

    window.draw(box);
}
