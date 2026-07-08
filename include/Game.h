#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <deque>
#include <string>

#include "GameObject.h"
#include "Player.h"
#include "Chunk.h"

class Game {
public:
    Game();

    void run();
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    sf::View camera;

    sf::RenderWindow window;
    sf::Clock clock;

    std::unique_ptr<Player> player;
    std::deque<std::unique_ptr<Chunk>> chunks;

    sf::Texture background_texture;
    // SFML 3 removed sf::Sprite’s default constructor
    // so we'll get an error if we say:
    // sf::Sprite player_sprite;
    std::optional<sf::Sprite> background_sprite; 


    void initGameObjects();
    void startGameObjects();
    void updateGameObjects();
    void renderGameObjects();
    void handleEvents();
    void lerpCameraPosition(float delta);
    void checkChunkGeneration();
    void handleChunkDeletion();
    void generateChunk();
};