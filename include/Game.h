#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameObject.h"

class Game {
public:
    Game();

    void run();
private:
    static constexpr int SCREEN_WIDTH = 720;
    static constexpr int SCREEN_HEIGHT = 1920;

    sf::RenderWindow window;
    sf::Clock clock;

    std::vector<std::unique_ptr<GameObject>> game_objects;

    void startGameObjects();
    void updateGameObjects();
    void handleEvents();
};