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
    sf::View camera;

    sf::RenderWindow window;
    sf::Clock clock;

    std::unique_ptr<Player> player;

    void initGameObjects();
    void startGameObjects();
    void updateGameObjects();
    void renderGameObjects();
    void handleEvents();
    void lerpCameraPosition(float delta);
};