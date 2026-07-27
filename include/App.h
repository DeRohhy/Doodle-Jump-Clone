#pragma once

#include <SFML/Graphics.hpp>
#include "scenes/SceneManager.h"
#include "GameConstants.h"
#include "MusicPlayer.h"

class App {
public:
    App() : window(
        sf::RenderWindow(sf::VideoMode({GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT}),
                         "Doodle Jump Clone")
        ) {}

    void run();
private:
    sf::RenderWindow window;
    SceneManager manager;
    sf::Clock clock;
    MusicPlayer music_player;
};