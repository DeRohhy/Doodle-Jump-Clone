#include "Game.h"

Game::Game() {
    window = sf::RenderWindow(sf::VideoMode({SCREEN_WIDTH, SCREEN_HEIGHT}),
                              "Doodle Jump Clone");
}

void Game::run() {
    startGameObjects();

    while (window.isOpen()) {
        handleEvents();
        
        updateGameObjects();

        window.clear();

        // TODO: render objects

        window.display();
    }
}

void Game::startGameObjects() {
    for (const auto& object: game_objects) {
        object->start();
    }
}

void Game::updateGameObjects() {
    float delta = clock.restart().asSeconds();
    for (const auto& object: game_objects) {
        object->update(delta);
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}