#include "Spring.h"

#include "singletons/ResourceManager.h"

#include "Theme.h"


Spring::Spring(sf::Vector2f _position)
    : GameObject(_position, sf::Vector2f()), compressed(true) {

    compressed_spring_texture = ResourceManager<sf::Texture>::getInstance().get(SPRING_COMPRESSED_PATH);
    expanded_spring_texture = ResourceManager<sf::Texture>::getInstance().get(SPRING_EXPANDED_PATH);

    compressed_spring.emplace(compressed_spring_texture);
    expanded_spring.emplace(expanded_spring_texture);
}

sf::FloatRect Spring::getBounds() {
    if (compressed && compressed_spring) {
        return compressed_spring->getGlobalBounds();
    } else if (!compressed && expanded_spring) {
        return expanded_spring->getGlobalBounds();
    }
    
    return sf::FloatRect();
}

void Spring::start() {
    sf::Vector2f compressed_local_bound = compressed_spring->getLocalBounds().size;
    compressed_spring->setOrigin({compressed_local_bound.x / 2.f, compressed_local_bound.y});
    compressed_spring->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});

    sf::Vector2f extended_local_bound = expanded_spring->getLocalBounds().size;
    expanded_spring->setOrigin({extended_local_bound.x / 2.f, extended_local_bound.y});
    expanded_spring->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});


    compressed_spring->setPosition(position);
    expanded_spring->setPosition(position);
}

void Spring::render(sf::RenderWindow& window) {
    if (compressed) {
        window.draw(compressed_spring.value());
    } else {
        window.draw(expanded_spring.value());
    }
}