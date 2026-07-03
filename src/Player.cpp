#include "Player.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"
#include <iostream>

void Player::start() {
    left_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(LEFT_DOODLE_PATH);
    right_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(RIGHT_DOODLE_PATH);

    player_sprite.emplace(left_doodle_texture); // construct in-place with texture
}

void Player::update(float delta) {
    velocity.y += GameConfig::GRAVITY * delta;
    position += velocity * delta;

    if (player_sprite) player_sprite->setPosition(position);
}

void Player::render(sf::RenderWindow& window) {
    if (player_sprite) window.draw(*player_sprite);
}
