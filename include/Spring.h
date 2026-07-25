#pragma once

#include "GameObject.h"

#include <memory>

class Spring : public GameObject {
public:
    Spring(sf::Vector2f _position);

    sf::FloatRect getBounds();

    bool isCompressed() { return compressed; }
    void setCompressed(bool value) { compressed = value; }

    void start() override;
    void render(sf::RenderWindow& window) override;
private:
    static inline const std::string SPRING_COMPRESSED_PATH = "assets/spring_compressed.png";
    static inline const std::string SPRING_EXPANDED_PATH = "assets/spring_expanded.png";

    bool compressed;

    sf::Texture compressed_spring_texture;
    sf::Texture expanded_spring_texture;
    std::optional<sf::Sprite> compressed_spring;
    std::optional<sf::Sprite> expanded_spring;
};