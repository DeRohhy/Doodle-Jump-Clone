#pragma once

#include "scenes/Scene.h"

#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
public:
    MainMenu(SceneManager& _manager);
    ~MainMenu() override = default;

    void start() override;
    void update(float /*delta*/) override {};
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    const std::string BUTTON_PATH = "assets/start_button.png";
    static constexpr float SPRITE_SCALE = 0.7f;

    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 
    
    sf::Font font;
    std::optional<sf::Text> title;
    std::optional<sf::Text> high_score;
    std::optional<sf::Text> tutorial;

    sf::Texture button_texture;
    std::optional<sf::Sprite> start_button;

    void positionText(sf::Text& text, float x, float y);
};