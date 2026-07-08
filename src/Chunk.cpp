#include "Chunk.h"
#include "singletons/GameConfig.h"
#include <memory>

void Chunk::start() {
    const float top_y = position.y;
    const float bottom_y = position.y + static_cast<float>(GameConfig::CHUNK_HEIGHT);

    float y = bottom_y - getRandomGap();
    while (y >= top_y) {
        float x = random_generator.randomFloatRange(0, GameConfig::SCREEN_WIDTH);

        std::unique_ptr new_platform = std::make_unique<NormalPlatform>(sf::Vector2f{x, y});
        new_platform->start();
        platforms.push_front(std::move(new_platform));

        y -= getRandomGap();
    }
}

void Chunk::update(float delta) {
    checkPlatformCollisions();
    removeOffScreenPlatforms();
    for (const auto& platform: platforms) {
        platform->update(delta);
    }
}

void Chunk::render(sf::RenderWindow& window) {
    for (const auto& platform: platforms) {
        platform->render(window);
    }    
}

inline float Chunk::getRandomGap() {
    return random_generator.randomFloatRange(GameConfig::MIN_OBJ_GAP, GameConfig::MAX_OBJ_GAP);
}

void Chunk::checkPlatformCollisions() {
    for (auto const& platform: platforms) {
        player->handlePlatformCollision(platform.get());
    }
}

void Chunk::removeOffScreenPlatforms() {
    const float camera_bottom_y = camera->getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);
    while (!platforms.empty() && platforms.back()->getPosition().y >= camera_bottom_y) {
        platforms.back().release();
        platforms.pop_back();
    }
}