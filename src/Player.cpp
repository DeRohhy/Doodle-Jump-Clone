#include "Player.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"
#include <iostream>
#include <math.h>

void Player::start() {
    left_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(LEFT_DOODLE_PATH);
    right_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(RIGHT_DOODLE_PATH);

    player_sprite.emplace(left_doodle_texture); // construct in-place with texture
}

void Player::update(float delta) {
    handleMovement(delta);
    velocity.y += GameConfig::GRAVITY * delta;
    position += velocity * delta;


    if (player_sprite) player_sprite->setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    if (player_sprite) window.draw(*player_sprite);
}

void Player::handleMovement(float delta) {
    int direction = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) direction = -1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) direction = 1;

    if (direction == 1 && !is_facing_right) {
        is_facing_right = true;
        player_sprite->setTexture(right_doodle_texture);
    } else if (direction == -1 && is_facing_right) {
        is_facing_right = false;
        player_sprite->setTexture(left_doodle_texture);
    }

    float target_speed = static_cast<float>(direction) * TOP_SPEED;
    float speed_difference = target_speed - velocity.x;
    float delta_velocity = std::pow(std::abs(speed_difference) * ACCEL_RATE, VELOCITY_POWER) * (speed_difference > 0 ? 1 : -1);

    velocity.x += delta_velocity * delta;
}
