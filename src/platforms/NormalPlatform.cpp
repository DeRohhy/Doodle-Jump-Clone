#include "platforms/Platform.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"

sf::FloatRect Platform::getBounds() {
    if (!platform_sprite) {
        return sf::FloatRect();
    }

    return platform_sprite->getGlobalBounds();
}

void Platform::start() {
    platform_texture = ResourceManager<sf::Texture>::getInstance().get(platform_path);

    platform_sprite.emplace(platform_texture);
    
    // offset platform sprite so that
    // the center of platform is at local x = 0
    sf::Vector2f local_bound = platform_sprite->getLocalBounds().size;
    platform_sprite->setOrigin({local_bound.x / 2.0f, 0.0f});

    platform_sprite->setScale({GameConfig::SPRITE_SCALE, GameConfig::SPRITE_SCALE});
    platform_sprite->setPosition(position);
}

void Platform::render(sf::RenderWindow& window) {
    if (platform_sprite) window.draw(*platform_sprite);
}