#include "Hole.h"

#include "singletons/ResourceManager.h"
#include "Theme.h"

Hole::Hole(sf::Vector2f _position, bool small_hole) : GameObject(_position, sf::Vector2f()) {
    hole_texture = ResourceManager<sf::Texture>::getInstance().get(SPRITE_PATH);

    hole_sprite.emplace(hole_texture);
    
    sf::Vector2f local_bound = hole_sprite->getLocalBounds().size;
    hole_sprite->setOrigin({local_bound.x / 2.f, local_bound.y / 2.f});

    if (small_hole) {
        hole_sprite->setScale({Theme::SCALE_SMALL, Theme::SCALE_SMALL});
    } else {
        hole_sprite->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});
    }
    hole_sprite->setPosition(position);
}

sf::FloatRect Hole::getBounds() {
    if (!hole_sprite) {
        return sf::FloatRect();
    }
    
    return hole_sprite->getGlobalBounds();
}

sf::Vector2f Hole::getCenterPoint() {
    return hole_sprite->getPosition();
}

void Hole::render(sf::RenderWindow& window) {
    if (hole_sprite) {
        window.draw(hole_sprite.value());
    }
}
