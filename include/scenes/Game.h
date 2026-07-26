#pragma once


#include <SFML/Graphics.hpp>

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
    Game(SceneManager& _manager);

    void start() override;
    void update(float delta) override;
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    static constexpr int BOTTOM_Y = GameConstants::SCREEN_HEIGHT - 100;
    static constexpr float CAMERA_TRIGGER_PERCENTAGE = 0.1f;
    static constexpr float CAMERA_LERP_SPEED = 15.f;

    sf::View camera;

    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 

    std::unique_ptr<Player> player;
    std::deque<std::unique_ptr<Chunk>> chunks;

    sf::Font font;
    std::optional<sf::Text> score_label;
    int score;    

    void lerpCameraPosition(float delta);
    void checkChunkGeneration();
    void handleChunkDeletion();
    void generateChunk();
};