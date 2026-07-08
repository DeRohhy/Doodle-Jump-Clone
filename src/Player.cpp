#include "Player.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"
#include <math.h>

sf::FloatRect Player::getBounds() {
    if (!player_sprite) {
        return sf::FloatRect();
    }

    return player_sprite->getGlobalBounds();
}

void Player::start() {
    right_doodle_texture = ResourceManager<sf::Texture>::getInstance().get(RIGHT_DOODLE_PATH);

    player_sprite.emplace(right_doodle_texture); // construct in-place with texture

    // offset player sprite so that
    // the bottom of the feet is at local y = 0
    // and the center between player legs is at local x = 0
    sf::Vector2f local_bound = player_sprite->getLocalBounds().size;
    static constexpr float offset_percentage = 3.f / 8.f;
    player_sprite->setOrigin({offset_percentage * local_bound.x, local_bound.y});

    player_sprite->setScale({SPRITE_SCALE, SPRITE_SCALE});
}

void Player::update(float delta) {
    handleScreenWrapping();
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

    if (direction != 0)
        setDirection(direction == -1 ? Direction::LEFT : Direction::RIGHT);

    float target_speed = static_cast<float>(direction) * TOP_SPEED;
    float speed_difference = target_speed - velocity.x;
    float delta_velocity = std::pow(std::abs(speed_difference) * ACCEL_RATE, VELOCITY_POWER) * (speed_difference > 0 ? 1 : -1);

    velocity.x += delta_velocity * delta;
}

void Player::handleJump() {
    velocity.y = -JUMP_FACTOR;
}


void Player::handlePlatformCollision(Platform* platform) {
    static constexpr float epsilon = 5.f;
    if (velocity.y < 0 || position.y > platform->getPosition().y + epsilon)
        return;

    // findIntersection returns std::optional<sf::FloatRect>
    if (!static_cast<bool>(getBounds().findIntersection(platform->getBounds())))
        return;

    position.y = platform->getPosition().y;
    handleJump();
}


void Player::setDirection(Direction new_direction) {
    if (facing_direction == new_direction) return; 
        
    facing_direction = new_direction;

    if (!player_sprite) return;

    if (new_direction == Direction::LEFT) {
        player_sprite->setScale({-SPRITE_SCALE, SPRITE_SCALE});
    } else {
        player_sprite->setScale({SPRITE_SCALE, SPRITE_SCALE});
    }
}

void Player::handleScreenWrapping() {
    static constexpr float margin = 10.f;
    sf::Vector2f center_of_feet_pos = position + player_sprite->getOrigin();

    if (center_of_feet_pos.x < margin) {
        position.x = GameConfig::SCREEN_WIDTH - player_sprite->getOrigin().x;
    } else if (center_of_feet_pos.x > GameConfig::SCREEN_WIDTH + margin) {
        position.x = 0;
    } 
}