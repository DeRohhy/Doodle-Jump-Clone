#pragma once

#include <SFML/Graphics.hpp>
#include <deque>

#include "singletons/Random.h"
#include "GameObject.h"

#include "platforms/BrokenPlatform.h"
#include "enemies/Enemy.h"
#include "Spring.h"
#include "Player.h"
#include "Hole.h"

class Chunk : public GameObject {
public:
    Chunk(sf::Vector2f _position, Player* _player, sf::View* _camera)
        : GameObject(_position, sf::Vector2f()),
          player(_player), camera(_camera) {}

    void start() override;
    void update(float delta) override;
    void render(sf::RenderWindow& window) override;

    float get_highest_platform_y() { return platforms.front()->getPosition().y; }

    bool isGameOver() { return game_over; }
private:
    static constexpr float SIDE_MARGIN = 50.f;

    static constexpr float MIN_OBJ_GAP = 100.f;
    static constexpr float MAX_OBJ_GAP = 275.f;

    static constexpr float BROKEN_PLATFORM_SPAWN_CHANCE = 0.1;
    static constexpr float MOVING_PLATFORM_SPAWN_CHANCE = 0.1;
    
    static constexpr int MAX_BROKEN_PLATFORM_ATTEMPTS = 2;
    
    static constexpr int MAX_SPRING_OFFSET = 15;
    static constexpr float SPRING_SPAWN_CHANCE = 0.1f;

    static constexpr float MIN_PLATFORM_MOVE_SPEED = 100.f;
    static constexpr float MAX_PLATFORM_MOVE_SPEED = 250.f;

    static constexpr float SPAWN_HAZARD_CHANCE = 0.5f;

    static constexpr float MIN_PLATFROM_GAP_FOR_HAZARD = 230.f;

    static constexpr float ENEMY_SPAWN_CHANCE = 0.3f;
    static constexpr float MOVING_ENEMY_SPAWN_CHANCE = 0.7f;
    static constexpr float MIN_MOVING_ENEMY_SPEED = 100.f;
    static constexpr float MAX_MOVING_ENEMY_SPEED = 160.f;

    static constexpr int ENEMY_HEALTH_EASY = 1;
    static constexpr int ENEMY_HEALTH_MEDIUM = 2;
    static constexpr int ENEMY_HEALTH_HARD = 3;

    static constexpr float SMALL_HOLE_SPAWN_CHANCE = 0.5f;
    

    Player* player;
    sf::View* camera;
    Random& random_generator = Random::getInstance();
    std::deque<std::unique_ptr<Platform>> platforms;
    std::deque<std::unique_ptr<BrokenPlatform>> broken_platforms;
    std::deque<std::unique_ptr<Spring>> springs;
    std::deque<std::unique_ptr<Enemy>> enemies;
    std::deque<std::unique_ptr<Bullet>>* bullets;
    std::deque<std::unique_ptr<Hole>> holes;

    void spawnRow(float y, const float last_gap);

    void tryGenerateBrokenPlatform(float y);

    float getRandomGap();
    void handleCollisions();
    void removeOffScreenObjects();

    void generateNormalPlatform(float x, float y, bool spawn_spring);
    void generateBrokenPlatform(float x, float y);
    void generateMovingPlatform(float x, float y);

    void generateEnemy(float y);
    void tryGenerateHole(float y);

    void generateMovingEnemy(float y);
    void generateStationaryEnemy(float y);

    bool game_over = false;
};