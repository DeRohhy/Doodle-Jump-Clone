#pragma once

#include "scenes/Scene.h"

#include <SFML/Graphics.hpp>

class GameOverMenu : public Scene {
public:
    GameOverMenu(SceneManager& _manager);
    ~GameOverMenu() override = default;

    void start() override;
    void update(float /*delta*/) override {};
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    const std::string RESTART_BUTTON_PATH = "assets/restart_button.png";
    const std::string MENU_BUTTON_PATH = "assets/menu_button.png";
    static constexpr float SPRITE_SCALE = 0.7f;

    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 
    
    sf::Font font;
    std::optional<sf::Text> title;
    std::optional<sf::Text> score;
    std::optional<sf::Text> high_score;

    sf::Texture restart_button_texture;
    sf::Texture menu_button_texture;
    std::optional<sf::Sprite> restart_button;
    std::optional<sf::Sprite> menu_button;

    void positionText(sf::Text& text, float x, float y);
};