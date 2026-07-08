#pragma once

#include <SFML/Graphics.hpp>
#include "GameObject.h"
#include "platforms/NormalPlatform.h"
#include "singletons/Random.h"
#include <vector>

class Chunk : public GameObject {
public:
    Chunk(sf::Vector2f _position) : GameObject(_position, sf::Vector2f()) {}

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    Random& random_generator = Random::getInstance();
    std::vector<std::unique_ptr<Platform>> platforms;

    inline float getRandomGap();
};