#pragma once


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <vector>
#include <memory>
#include <deque>
#include <string>

#include "GameConstants.h"
#include "scenes/Scene.h"
#include "Player.h"
#include "Chunk.h"

class Game : public Scene {
public:
    Game(SceneManager& _manager, MusicPlayer* _music_player);

    void start() override;
    void update(float delta) override;
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    static constexpr int BOTTOM_Y = GameConstants::SCREEN_HEIGHT - 100;

    sf::View camera;
    
    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 

    std::unique_ptr<Player> player;
    std::deque<std::unique_ptr<Chunk>> chunks;

    sf::Font font;
    std::optional<sf::Text> score_label;
    int score;

    void updateCameraPosition();
    void checkChunkGeneration();
    void handleChunkDeletion();
    void generateChunk();
    void handleGameOver();
};