#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>

#include "GameObject.h"
#include "Player.h"
#include "platforms/NormalPlatform.h"

class Game {
public:
    Game();

    void run();
private:
    static constexpr unsigned int SCREEN_WIDTH = 600;
    static constexpr unsigned int SCREEN_HEIGHT = 800;

    sf::RenderWindow window;
    sf::Clock clock;

    std::unique_ptr<Player> player;
    std::vector<std::unique_ptr<Platform>> platforms;

    std::vector<GameObject*> game_objects;

    void initGameObjects();
    void startGameObjects();
    void updateGameObjects();
    void renderGameObjects();
    void handleEvents();
    void CheckPlatformCollisons();
};