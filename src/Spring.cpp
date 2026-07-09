#include "Spring.h"

#include "singletons/ResourceManager.h"


Spring::Spring(sf::Vector2f _position)
    : GameObject(_position, sf::Vector2f()), compressed(false) {
    
    spring_texture = ResourceManager<sf::Texture>::getInstance().get(SPRING_SPRTIE_PATH);
    const int texture_width = static_cast<int>(spring_texture.getSize().x);

    compressed_spring.emplace(spring_texture);
    compressed_spring->setTextureRect(sf::IntRect({0, 0}, {texture_width, COMPRESSED_SPRING_HEIGHT}));

    extended_spring.emplace(spring_texture);
    extended_spring->setTextureRect(sf::IntRect({0, COMPRESSED_SPRING_HEIGHT}, {texture_width, COMPRESSED_SPRING_HEIGHT}));
}

sf::FloatRect Spring::getBounds() {
    if (compressed && compressed_spring) {
        return compressed_spring->getGlobalBounds();
    } else if (!compressed && extended_spring) {
        return extended_spring->getGlobalBounds();
    }
    
    return sf::FloatRect();
}

void Spring::start() {
    static constexpr int EPSILON = 5;

    sf::Vector2f compressed_local_bound = compressed_spring->getLocalBounds().size;
    compressed_spring->setOrigin({compressed_local_bound.x / 2.f, compressed_local_bound.y - EPSILON});

    sf::Vector2f extended_sprite_local_bound = extended_spring->getLocalBounds().size;
    extended_spring->setOrigin({extended_sprite_local_bound.x / 2.f, extended_sprite_local_bound.y - EPSILON});

    compressed_spring->setPosition(position);
    extended_spring->setPosition(position);
}

void Spring::render(sf::RenderWindow& window) {
    if (compressed) {
        window.draw(compressed_spring.value());
    } else {
        window.draw(extended_spring.value());
    }
}