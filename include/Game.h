#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameObject.h"
#include "Player.h"

class Game {
public:
    Game();

    void run();
private:
    static constexpr int SCREEN_HEIGHT = 720;
    static constexpr int SCREEN_WIDTH = 1280;

    sf::RenderWindow window;
    sf::Clock clock;

    std::unique_ptr<Player> player;

    std::vector<GameObject*> game_objects;

    void initGameObjects();
    void startGameObjects();
    void updateGameObjects();
    void renderGameObjects();
    void handleEvents();
};