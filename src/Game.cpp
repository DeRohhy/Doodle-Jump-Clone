#include "Game.h"

Game::Game() {
    window = sf::RenderWindow(sf::VideoMode({SCREEN_HEIGHT, SCREEN_WIDTH}),
                              "Doodle Jump Clone");
}

void Game::run() {
    initGameObjects();
    startGameObjects();

    while (window.isOpen()) {
        handleEvents();
        updateGameObjects();

        window.clear();

        renderGameObjects();

        window.display();
    }
}

void Game::initGameObjects() {
    player = std::make_unique<Player>(sf::Vector2f(), sf::Vector2f());

    game_objects.push_back(player.get());

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

void Game::renderGameObjects() {
    for (const auto& object: game_objects) {
        object->render(window);
    }
}

void Game::handleEvents() {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}