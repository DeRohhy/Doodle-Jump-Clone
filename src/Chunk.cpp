#include "Chunk.h"
#include "singletons/GameConfig.h"
#include "platforms/NormalPlatform.h"
#include "platforms/MovingPlatform.h"

#include <memory>
#include <algorithm>

void Chunk::start() {
    const float top_y = position.y;
    const float bottom_y = position.y + static_cast<float>(GameConfig::CHUNK_HEIGHT);
    static const float side_margin = 50.f;

    float y = bottom_y - getRandomGap();
    while (y >= top_y) {
        float x = random_generator.randomFloatRange(side_margin, GameConfig::SCREEN_WIDTH - side_margin);

        static const float BROKEN_PLATFORM_SPAWN_CHANCE = 0.1;
        static const float MOVING_PLATFORM_SPAWN_CHANCE = 0.1;
        static const float SPRINTG_SPAWN_CHANCE = 0.1;
 
        bool spawn_moving_platform = random_generator.randomFloatRange(0, 1) < MOVING_PLATFORM_SPAWN_CHANCE;

        if (spawn_moving_platform) {

            generateMovingPlatform(x, y);
        } else {
            bool spawn_spring = random_generator.randomFloatRange(0, 1) <= SPRINTG_SPAWN_CHANCE;
            generateNormalPlatform(x, y, spawn_spring);

            bool spawn_borken_platform = random_generator.randomFloatRange(0, 1) < BROKEN_PLATFORM_SPAWN_CHANCE;

            if (spawn_borken_platform) {
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
    handleCollisions();
    removeOffScreenObjects();
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
    for (const auto& spring: springs) {
        spring->render(window);
    } 
}

inline float Chunk::getRandomGap() {
    return random_generator.randomFloatRange(GameConfig::MIN_OBJ_GAP, GameConfig::MAX_OBJ_GAP);
}

void Chunk::handleCollisions() {
    for (auto const& platform: platforms) {
        if (player->isColliding(platform.get()->getBounds())) {
            player->handleJump();
        }
    }

    for (auto it = broken_platforms.begin(); it != broken_platforms.end(); ++it) {
        if (player->isColliding(it->get()->getBounds())) {
            broken_platforms.erase(it);
            break;
        }
    }

    for (auto const& spring: springs) {
        if (player->isColliding(spring.get()->getBounds())) {
            player->handleSpringJump();
            spring->setCompressed(true);
        }
    }
}



void Chunk::removeOffScreenObjects() {
    const float camera_bottom_y = camera->getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);
    while (!platforms.empty() && platforms.back()->getPosition().y >= camera_bottom_y) {
        platforms.pop_back();
    }

    while (!broken_platforms.empty() && broken_platforms.back()->getPosition().y >= camera_bottom_y) {
        broken_platforms.pop_back();
    }

    while (!springs.empty() && springs.back()->getPosition().y >= camera_bottom_y) {
        springs.pop_back();
    }
}

void Chunk::generateNormalPlatform(float x, float y, bool spawn_spring) {
    std::unique_ptr new_platform = std::make_unique<NormalPlatform>(sf::Vector2f{x, y});
    new_platform->start();
    
    if (spawn_spring) {
        sf::Vector2f spring_position;
        
        spring_position.y = new_platform->getPosition().y;

        static const int MAX_SPRING_OFFSET = 15;
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
    static const float MIN_SPEED = 100;
    static const float MAX_SPEED = 250;
    const float speed = random_generator.randomFloatRange(MIN_SPEED, MAX_SPEED);
    std::unique_ptr new_platform = std::make_unique<MovingPlatform>(sf::Vector2f{x, y}, speed);
    new_platform->start();
    platforms.push_front(std::move(new_platform));    
}

