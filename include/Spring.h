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
    static inline const std::string PLATFORM_SPRITE_PATH = "assets/normal_platform.png";
    static inline const std::string SPRING_SPRTIE_PATH = "assets/spring_sprite.png";
    static constexpr int COMPRESSED_SPRING_HEIGHT = 21;

    bool compressed;

    sf::Texture spring_texture;
    std::optional<sf::Sprite> compressed_spring;
    std::optional<sf::Sprite> extended_spring;
};