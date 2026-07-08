#include "Game.h"
#include "singletons/GameConfig.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Game::Game() {
    window = sf::RenderWindow(sf::VideoMode({GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT}),
                              "Doodle Jump Clone");

    camera = sf::View(sf::FloatRect(
        {0, 0},
        {GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT}
    ));
}

void Game::run() {
    initGameObjects();
    startGameObjects();

    while (window.isOpen()) {
        handleEvents();
        updateGameObjects();
        checkChunkGeneration();
        handleChunkDeletion();

        window.clear();
        window.setView(camera);

        renderGameObjects();

        // Reset the view to default for drawing static UI elements
        window.setView(window.getDefaultView());
        // ...

        window.display();
    }
}

void Game::initGameObjects() {
    player = std::make_unique<Player>(
        sf::Vector2f{GameConfig::SCREEN_WIDTH / 2, GameConfig::SCREEN_HEIGHT / 2},
        sf::Vector2f()
    );

    // generate 2 starting chunks
    generateChunk();
    generateChunk();
}

void Game::startGameObjects() {
    player->start();
}

void Game::updateGameObjects() {
    float delta = clock.restart().asSeconds();
    player->update(delta);
    for (const auto& chunk: chunks) {
        chunk->update(delta);
    }
    lerpCameraPosition(delta);
}

void Game::renderGameObjects() {
    player->render(window);
    for (const auto& chunk: chunks) {
        chunk->render(window);
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::lerpCameraPosition(float delta) {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y 
                            - GameConfig::CAMERA_TRIGGER_PERCENTAGE * (GameConfig::SCREEN_HEIGHT / 2.f);

    if (target_y >= threshold) {
        return;
    }

    // framerate-independent lerp
    const float blend = 1.f - std::exp(-GameConfig::CAMERA_LERP_SPEED * delta);
    const float new_y = camera_center_y + (target_y - threshold) * blend;

    camera.setCenter({camera.getCenter().x, new_y});
}

void Game::checkChunkGeneration() {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y 
                            - GameConfig::CAMERA_TRIGGER_PERCENTAGE * (GameConfig::SCREEN_HEIGHT / 2.f);
    
    if (target_y >= threshold || chunks.size() >= 3) {
        return;
    }

    generateChunk();
}

void Game::handleChunkDeletion() {
    const float camera_bottom_y = camera.getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);

    while (!chunks.empty() && chunks.back()->getPosition().y >= camera_bottom_y) {
        chunks.back().release();
        chunks.pop_back();
    }
}

void Game::generateChunk() {
    const float chunk_position_y = 
        chunks.empty() ? GameConfig::SCREEN_HEIGHT / 2.f : 
                         chunks.front()->getPosition().y - GameConfig::CHUNK_HEIGHT;
    
    std::unique_ptr<Chunk> new_chunk = std::make_unique<Chunk>(
        sf::Vector2f({0, chunk_position_y}),
        player.get()
    );
    new_chunk->start();
    
    chunks.push_front(std::move(new_chunk));
}