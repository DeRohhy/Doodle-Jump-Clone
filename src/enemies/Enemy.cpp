#include "enemies/Enemy.h"

#include "singletons/ResourceManager.h"
#include "Theme.h"

sf::FloatRect Enemy::getBounds() {
    if (!enemy_sprite) {
        return sf::FloatRect();
    }
    
    return enemy_sprite->getGlobalBounds();
}

void Enemy::start() {
    enemy_texture = ResourceManager<sf::Texture>::getInstance().get(enemy_path);

    enemy_sprite.emplace(enemy_texture);
    
    // offset enemy sprite so that
    // the center of enemy is at local x = 0
    sf::Vector2f local_bound = enemy_sprite->getLocalBounds().size;
    enemy_sprite->setOrigin({local_bound.x / 2.0f, 0.0f});

    enemy_sprite->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});
    enemy_sprite->setPosition(position);
}

void Enemy::render(sf::RenderWindow& window) {
    if (enemy_sprite) window.draw(enemy_sprite.value());
}