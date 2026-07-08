#include "Chunk.h"
#include "singletons/GameConfig.h"
#include "platforms/NormalPlatform.h"
#include "platforms/MovingPlatform.h"

#include <memory>
#include <algorithm>
#include <iostream>

void Chunk::start() {
    const float top_y = position.y;
    const float bottom_y = position.y + static_cast<float>(GameConfig::CHUNK_HEIGHT);
    static const float side_margin = 50.f;

    float y = bottom_y - getRandomGap();
    while (y >= top_y) {
        float x = random_generator.randomFloatRange(side_margin, GameConfig::SCREEN_WIDTH - side_margin);

        const float BROKEN_PLATFORM_SPAWN_CHANCE = 0.1;
        const float MOVING_PLATFORM_SPAWN_CHANCE = 0.1;

        float moving_spawn_chance = random_generator.randomFloatRange(0, 1);

        if (moving_spawn_chance < MOVING_PLATFORM_SPAWN_CHANCE) {
            generateMovingPlatform(x, y);
        } else {
            generateNormalPlatform(x, y);

            float broken_spawn_chance = random_generator.randomFloatRange(0, 1);

            if (broken_spawn_chance < BROKEN_PLATFORM_SPAWN_CHANCE) {
                const float max_attempts = 2;

                for (int i = 0; i < max_attempts; i++) {
                    float broken_platform_x = random_generator.randomFloatRange(side_margin, GameConfig::SCREEN_WIDTH - side_margin);

                    generateBrokenPlatform(broken_platform_x, y);              
                }
            }

        }



        

        y -= getRandomGap();
    }
}

void Chunk::update(float delta) {
    checkPlatformCollisions();
    removeOffScreenPlatforms();
    for (const auto& platform: platforms) {
        platform->update(delta);
    }
    for (const auto& platform: broken_platforms) {
        platform->update(delta);
    }
}

void Chunk::render(sf::RenderWindow& window) {
    for (const auto& platform: platforms) {
        platform->render(window);
    }
    for (const auto& platform: broken_platforms) {
        platform->render(window);
    }    
}

inline float Chunk::getRandomGap() {
    return random_generator.randomFloatRange(GameConfig::MIN_OBJ_GAP, GameConfig::MAX_OBJ_GAP);
}

void Chunk::checkPlatformCollisions() {
    for (auto const& platform: platforms) {
        if (player->isCollidingWithPlatform(platform.get())) {
            player->handleJump();
        }
    }

    for (auto it = broken_platforms.begin(); it != broken_platforms.end(); ++it) {
        if (player->isCollidingWithPlatform(it->get())) {
            broken_platforms.erase(it);
            break;
        }
    }
}

void Chunk::removeOffScreenPlatforms() {
    const float camera_bottom_y = camera->getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);
    while (!platforms.empty() && platforms.back()->getPosition().y >= camera_bottom_y) {
        platforms.back().release();
        platforms.pop_back();
    }

    while (!broken_platforms.empty() && broken_platforms.back()->getPosition().y >= camera_bottom_y) {
        broken_platforms.back().release();
        broken_platforms.pop_back();
    }
}

void Chunk::generateNormalPlatform(float x, float y) {
    std::unique_ptr new_platform = std::make_unique<NormalPlatform>(sf::Vector2f{x, y});
    new_platform->start();
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
    static const float MIN_SPEED = 100;
    static const float MAX_SPEED = 250;
    const float speed = random_generator.randomFloatRange(MIN_SPEED, MAX_SPEED);
    std::unique_ptr new_platform = std::make_unique<MovingPlatform>(sf::Vector2f{x, y}, speed);
    new_platform->start();
    platforms.push_front(std::move(new_platform));    
}
