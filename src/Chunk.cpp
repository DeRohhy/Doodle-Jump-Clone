#include "Chunk.h"
#include "singletons/GameSettings.h"
#include "platforms/NormalPlatform.h"
#include "platforms/MovingPlatform.h"

#include "GameConstants.h"

#include "enemies/MovingEnemy.h"
#include "enemies/StationaryEnemy.h"

#include <memory>
#include <algorithm>
#include <iostream>

void Chunk::start() {
    bullets = &player->getBulletsVectorRefrence();

    const float top_y = position.y;
    const float bottom_y = position.y + static_cast<float>(GameConstants::CHUNK_HEIGHT);

    float y = bottom_y - getRandomGap();
    float last_gap = 0;
    while (y >= top_y) {
        spawnRow(y, last_gap);

        last_gap = getRandomGap();
        y -= last_gap;       
    }
}

void Chunk::update(float delta) {
    handleCollisions();
    removeOffScreenObjects();
    for (const auto& platform: platforms) {
        platform->update(delta);
    }
    for (const auto& platform: broken_platforms) {
        platform->update(delta);
    }
    for (const auto& enemy: enemies) {
        enemy->update(delta);
    }
    for (const auto& bullet: *bullets) {
        bullet->update(delta);
    }
}

void Chunk::render(sf::RenderWindow& window) {
    for (const auto& platform: platforms) {
        platform->render(window);
    }
    for (const auto& platform: broken_platforms) {
        platform->render(window);
    }
    for (const auto& spring: springs) {
        spring->render(window);
    } 
    for (const auto& enemy: enemies) {
        enemy->render(window);
    }
    for (const auto& bullet: *bullets) {
        bullet->render(window);
    }
}

void Chunk::spawnRow(float y, const float last_gap) {
    const float x = random_generator.randomFloatRange(SIDE_MARGIN, GameConstants::SCREEN_WIDTH - SIDE_MARGIN);

    const bool spawn_moving_platform = random_generator.randomFloatRange(0, 1) < MOVING_PLATFORM_SPAWN_CHANCE;
    if (spawn_moving_platform) {
        generateMovingPlatform(x, y);
        return;
    }

    const bool spawn_spring = random_generator.randomFloatRange(0, 1) < SPRING_SPAWN_CHANCE;
    generateNormalPlatform(x, y, spawn_spring);


    const bool spawn_broken_platform = random_generator.randomFloatRange(0, 1) < BROKEN_PLATFORM_SPAWN_CHANCE;
    if (spawn_broken_platform) {
        tryGenerateBrokenPlatform(y);
    }

    const bool spawn_enemy = random_generator.randomFloatRange(0, 1) < ENEMY_SPAWN_CHANCE;
    if (spawn_enemy && last_gap >= MIN_PLATFROM_GAP_FOR_ENEMY) {
        const float enemy_y = y + last_gap / 2;
        generateEnemy(enemy_y);
    }
    
}

void Chunk::tryGenerateBrokenPlatform(float y) {
    for (int i = 0; i < MAX_BROKEN_PLATFORM_ATTEMPTS; i++) {
        const float x = random_generator.randomFloatRange(SIDE_MARGIN, GameConstants::SCREEN_WIDTH - SIDE_MARGIN);
        generateBrokenPlatform(x, y);
    }
}

float Chunk::getRandomGap() {
    return random_generator.randomFloatRange(MIN_OBJ_GAP, MAX_OBJ_GAP);
}

void Chunk::handleCollisions() {
    const sf::FloatRect player_feet = player->getFeetBounds();
    const sf::FloatRect player_body = player->getBodyBounds();
    const bool is_player_falling = player->getVelocity().y > 0;

    for (auto const& platform: platforms) {
        bool is_colliding = static_cast<bool>(player_feet.findIntersection(platform->getBounds()));
        if (is_colliding && is_player_falling) {
            player->handleJump();
        }
    }

    for (auto it = broken_platforms.begin(); it != broken_platforms.end(); ++it) {
        bool is_colliding = static_cast<bool>(player_feet.findIntersection((*it)->getBounds()));
        if (is_colliding && is_player_falling) {
            broken_platforms.erase(it);
            break;
        }
    }

    for (auto const& spring: springs) {
        bool is_colliding = static_cast<bool>(player_feet.findIntersection(spring->getBounds()));
        if (is_colliding && is_player_falling) {
            player->handleSpringJump();
            spring->setCompressed(false);
        }
    }
    
    for (auto const& enemy: enemies) {
        bool is_foot_colliding = static_cast<bool>(player_feet.findIntersection(enemy->getBounds()));
        bool is_body_colliding = static_cast<bool>(player_body.findIntersection(enemy->getBounds()));
        if (is_foot_colliding && is_player_falling) {
            player->handleSpringJump();
        } else if (is_body_colliding) {
            game_over = true;
        }
    }


    for (auto it = bullets->begin(); it != bullets->end(); ++it) {
        sf::FloatRect bullet_bounds = (*it)->getBounds();

        bool is_colliding = false;
        for (auto const& enemy: enemies) {
            is_colliding = static_cast<bool>(enemy->getBounds().findIntersection(bullet_bounds));
            if (is_colliding) {
                enemy->decrementHealth();
                bullets->erase(it);
                break;    
            }
        }

        if (is_colliding) {
            break;
        }
    }

    // remove dead enemies
    for (auto it = enemies.begin(); it != enemies.end(); ++it)
    {
        if ((*it)->isDead()) {
            enemies.erase(it);
            break;
        }
    }
}

void Chunk::removeOffScreenObjects() {
    const float camera_top_y = camera->getCenter().y - (GameConstants::SCREEN_HEIGHT / 2.f);
    const float camera_bottom_y = camera->getCenter().y + (GameConstants::SCREEN_HEIGHT / 2.f);
    while (!platforms.empty() && platforms.back()->getPosition().y >= camera_bottom_y) {
        platforms.pop_back();
    }

    while (!broken_platforms.empty() && broken_platforms.back()->getPosition().y >= camera_bottom_y) {
        broken_platforms.pop_back();
    }

    while (!springs.empty() && springs.back()->getPosition().y >= camera_bottom_y) {
        springs.pop_back();
    }

    while (!enemies.empty() && enemies.back()->getPosition().y - enemies.back()->getBounds().size.y >= camera_bottom_y) {
        enemies.pop_back();
    }

    while (!bullets->empty() && bullets->back()->getPosition().y <= camera_top_y) {
        bullets->pop_back();
    }
}

void Chunk::generateNormalPlatform(float x, float y, bool spawn_spring) {
    std::unique_ptr new_platform = std::make_unique<NormalPlatform>(sf::Vector2f{x, y});
    new_platform->start();
    
    if (spawn_spring) {
        sf::Vector2f spring_position;
        
        spring_position.y = new_platform->getPosition().y;

        float offset_x = Random::getInstance().randomFloatRange(-MAX_SPRING_OFFSET, MAX_SPRING_OFFSET);
        spring_position.x = new_platform->getPosition().x + offset_x;

        std::unique_ptr new_spring = std::make_unique<Spring>(spring_position);
        new_spring->start();
        springs.push_front(std::move(new_spring));
    }

    platforms.push_front(std::move(new_platform));
}

void Chunk::generateBrokenPlatform(float x, float y) {
    std::unique_ptr new_platform = std::make_unique<BrokenPlatform>(sf::Vector2f{x, y});
    new_platform->start();
    sf::FloatRect new_bounds = new_platform->getBounds();

    for (const auto& platform : platforms) {
        if (new_bounds.findIntersection(platform->getBounds())) {
            return;  // Overlaps, don't add
        }
    }

    for (const auto& platform : broken_platforms) {
        if (new_bounds.findIntersection(platform->getBounds())) {
            return;  // Overlaps, don't add
        }
    }
    
    broken_platforms.push_front(std::move(new_platform));
}

void Chunk::generateMovingPlatform(float x, float y) {
    const float speed = random_generator.randomFloatRange(MIN_PLATFORM_MOVE_SPEED, MAX_PLATFORM_MOVE_SPEED);
    std::unique_ptr new_platform = std::make_unique<MovingPlatform>(sf::Vector2f{x, y}, speed);
    new_platform->start();
    platforms.push_front(std::move(new_platform));    
}

void Chunk::generateEnemy(float y) {
    const bool spawn_moving_enemy = random_generator.randomFloatRange(0, 1) < MOVING_ENEMY_SPAWN_CHANCE;

    int health;
    switch (GameSettings::getInstance().getDifficulty())
    {
    default:
    case Difficulty::EASY:
        health = ENEMY_HEALTH_EASY;
        break;
    case Difficulty::MEDIUM:
        health = ENEMY_HEALTH_MEDIUM;
        break;
    case Difficulty::HARD:
        health = ENEMY_HEALTH_HARD;
        break;
    }

    const sf::Vector2f enemy_position = {random_generator.randomFloatRange(SIDE_MARGIN, GameConstants::SCREEN_WIDTH - SIDE_MARGIN), y};

    if (spawn_moving_enemy) {
        const float speed = random_generator.randomFloatRange(MIN_MOVING_ENEMY_SPEED, MAX_MOVING_ENEMY_SPEED);
        std::unique_ptr new_enemy = std::make_unique<MovingEnemy>(enemy_position, speed, health);
        new_enemy->start();
        enemies.push_front(std::move(new_enemy));
    } else {
        std::unique_ptr new_enemy = std::make_unique<StationaryEnemy>(enemy_position, health);
        new_enemy->start();
        enemies.push_front(std::move(new_enemy));
    }
}