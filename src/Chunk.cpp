#include "Chunk.h"
#include "singletons/GameConfig.h"
#include "platforms/NormalPlatform.h"
#include "platforms/MovingPlatform.h"

#include <memory>
#include <algorithm>

void Chunk::start() {
    const float top_y = position.y;
    const float bottom_y = position.y + static_cast<float>(GameConfig::CHUNK_HEIGHT);

    float y = bottom_y - getRandomGap();
    while (y >= top_y) {
        spawnRow(y);

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

void Chunk::spawnRow(float y) {
    const float x = random_generator.randomFloatRange(SIDE_MARGIN, GameConfig::SCREEN_WIDTH - SIDE_MARGIN);

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
}

void Chunk::tryGenerateBrokenPlatform(float y) {
    for (int i = 0; i < MAX_BROKEN_PLATFORM_ATTEMPTS; i++) {
        const float x = random_generator.randomFloatRange(SIDE_MARGIN, GameConfig::SCREEN_WIDTH - SIDE_MARGIN);
        generateBrokenPlatform(x, y);
    }
}

float Chunk::getRandomGap() {
    return random_generator.randomFloatRange(GameConfig::MIN_OBJ_GAP, GameConfig::MAX_OBJ_GAP);
}

void Chunk::handleCollisions() {
    for (auto const& platform: platforms) {
        if (player->isColliding(platform->getBounds()) && player->getVelocity().y > 0)  {
            player->handleJump();
        }
    }

    for (auto it = broken_platforms.begin(); it != broken_platforms.end(); ++it) {
        if (player->isColliding(it->get()->getBounds()) && player->getVelocity().y > 0) {
            broken_platforms.erase(it);
            break;
        }
    }

    for (auto const& spring: springs) {
        if (player->isColliding(spring->getBounds()) && player->getVelocity().y > 0) {
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

        static constexpr int MAX_SPRING_OFFSET = 15;
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
    const float speed = random_generator.randomFloatRange(GameConfig::MIN_PLATFORM_MOVE_SPEED, GameConfig::MAX_PLATFORM_MOVE_SPEED);
    std::unique_ptr new_platform = std::make_unique<MovingPlatform>(sf::Vector2f{x, y}, speed);
    new_platform->start();
    platforms.push_front(std::move(new_platform));    
}

