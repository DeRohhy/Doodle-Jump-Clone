#pragma once

#include "scenes/Scene.h"

#include <SFML/Graphics.hpp>

class SettingsMenu : public Scene {
public:
    SettingsMenu(SceneManager& _manager);
    ~SettingsMenu() override = default;

    void start() override;
    void update(float /*delta*/) override {};
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";

    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 

    sf::Font font;
    std::optional<sf::Text> title;
    std::optional<sf::Text> volume_title;
    std::optional<sf::Text> volume_percentage;
    std::optional<sf::Text> difficulty;
    


};