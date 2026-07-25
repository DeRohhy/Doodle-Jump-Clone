#include "scenes/Scene.h"

#include <cmath>

void Scene::makeText(std::optional<sf::Text>& slot,
                     const std::string& str,
                     const sf::Font& font,
                     int font_size,
                     sf::Color color,
                     sf::Text::Style style)
{   
    slot.emplace(font);
    slot->setString(str);
    slot->setCharacterSize(font_size);
    slot->setFillColor(color);
    slot->setStyle(style);
    sf::Vector2f local_bounds = slot->getLocalBounds().size;
    slot->setOrigin({local_bounds.x / 2, local_bounds.y / 2});
}
 
void Scene::makeButton(std::optional<sf::Sprite>& slot, const sf::Texture& texture,
                float sprite_scale)
{
    slot.emplace(texture);
    slot->setScale({sprite_scale, sprite_scale});
    sf::Vector2f local_bounds = slot->getLocalBounds().size;
    slot->setOrigin({local_bounds.x / 2, local_bounds.y / 2});
}

void Scene::makeLine(sf::RectangleShape& slot, sf::Vector2f from, sf::Vector2f to, sf::Color color, float thickness) {
    sf::Vector2f diff = to - from;
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float angle = std::atan2(diff.y, diff.x);

    slot = sf::RectangleShape(sf::Vector2f(length, thickness));
    slot.setPosition(from);
    slot.setRotation(sf::degrees(angle));
    slot.setFillColor(color);
}