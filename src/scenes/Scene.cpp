#include "scenes/Scene.h"

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

