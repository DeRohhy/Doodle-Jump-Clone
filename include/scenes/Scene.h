#pragma once

#include <SFML/Graphics.hpp>

class SceneManager;

class Scene {
public:
    explicit Scene(SceneManager& _manager) : manager(_manager) {}
    virtual ~Scene() = default;

    virtual void start() = 0;
    virtual void update(float delta) = 0;
    virtual void handleEvents(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
protected:
    SceneManager& manager;

    void makeText(std::optional<sf::Text>& slot,
                  const std::string& str,
                  const sf::Font& font,
                  int font_size,
                  sf::Color color,
                  sf::Text::Style style);

    void makeButton(std::optional<sf::Sprite>& slot, const sf::Texture& texture,
                    float sprite_scale);

    void makeLine(sf::RectangleShape& slot, sf::Vector2f from, sf::Vector2f to, sf::Color color, float thickness);
};
