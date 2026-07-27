#pragma once

#include "scenes/Scene.h"
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
public:
    MainMenu(SceneManager& _manager, MusicPlayer* _music_player);
    ~MainMenu() override = default;

    void start() override;
    void update(float /*delta*/) override {};
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    const std::string START_BUTTON_PATH = "assets/start_button.png";
    const std::string SETTINGS_BUTTON_PATH = "assets/settings_button.png";
    const std::string MAIN_MENU_MUSIC_PATH = "sounds/main_music_song.wav";


    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 
    
    sf::Font font;
    std::optional<sf::Text> title;
    std::optional<sf::Text> high_score;
    std::optional<sf::Text> mode;
    std::optional<sf::Text> tutorial_move;
    std::optional<sf::Text> tutorial_shoot;

    sf::Texture start_button_texture;
    std::optional<sf::Sprite> start_button;

    sf::Texture settings_button_texture;
    std::optional<sf::Sprite> settings_button;


};