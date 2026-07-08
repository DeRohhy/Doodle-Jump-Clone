#pragma once

#include <SFML/Graphics.hpp>
#include "scenes/SceneManager.h"
#include "singletons/GameConfig.h"

class App {
public:
    App() : window(
        sf::RenderWindow(sf::VideoMode({GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT}),
                         "Doodle Jump Clone")
        ) {}

    void run();
private:
    sf::RenderWindow window;
    SceneManager manager;
    sf::Clock clock;
};