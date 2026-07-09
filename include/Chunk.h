#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

#include "singletons/Random.h"
#include "GameObject.h"
#include "platforms/BrokenPlatform.h"
#include "Spring.h"
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
    Player* player;
    sf::View* camera;
    Random& random_generator = Random::getInstance();
    std::deque<std::unique_ptr<Platform>> platforms;
    std::deque<std::unique_ptr<BrokenPlatform>> broken_platforms;
    std::deque<std::unique_ptr<Spring>> springs;

    inline float getRandomGap();
    void handleCollisions();
    void removeOffScreenObjects();

    void generateNormalPlatform(float x, float y, bool spawn_spring);
    void generateBrokenPlatform(float x, float y);
    void generateMovingPlatform(float x, float y);
};