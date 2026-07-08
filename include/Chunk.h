#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

#include "singletons/Random.h"
#include "GameObject.h"
#include "platforms/BrokenPlatform.h"
#include "Player.h"

class Chunk : public GameObject {
public:
    Chunk(sf::Vector2f _position, Player* _player, sf::View* _camera)
        : GameObject(_position, sf::Vector2f()),
          player(_player), camera(_camera) {}

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

private:
    static constexpr int HAZARD_TYPE_COUNT = 3;
    Player* player;
    sf::View* camera;
    Random& random_generator = Random::getInstance();
    std::deque<std::unique_ptr<Platform>> platforms;
    std::deque<std::unique_ptr<BrokenPlatform>> broken_platforms;

    inline float getRandomGap();
    void checkPlatformCollisions();
    void removeOffScreenPlatforms();

    void generateNormalPlatform(float x, float y);
    void generateBrokenPlatform(float x, float y);
    void generateMovingPlatform(float x, float y);
};