#pragma once

#include "scenes/Scene.h"

#include <SFML/Graphics.hpp>

class SettingsMenu : public Scene {
public:
    SettingsMenu(SceneManager& _manager, MusicPlayer* _music_player);
    ~SettingsMenu() override = default;

    void start() override;
    void update(float /*delta*/) override {};
    void handleEvents(sf::RenderWindow& window) override;
    void render(sf::RenderWindow& window) override;
private:
    const std::string BACKGROUND_PATH = "assets/background.png";
    const std::string FONT_PATH = "assets/ariblk.ttf";
    const std::string BACK_BUTTON_PATH = "assets/back_button.png";
    static constexpr float VOLUME_SLIDER_LEN = 350.f;
    static constexpr float VOLUME_SLIDER_THICKNESS = 10.f;
    static constexpr float CHECK_BOX_RADIUS = 7.f;

    sf::Texture background_texture;
    std::optional<sf::Sprite> background_sprite; 

    sf::Font font;
    std::optional<sf::Text> title;
    
    std::optional<sf::Text> volume;
    std::optional<sf::Text> volume_percentage;
    sf::RectangleShape volume_bar;
    sf::RectangleShape volume_background;

    std::optional<sf::Text> difficulty;
    sf::CircleShape easy_check_box;
    sf::CircleShape medium_check_box;
    sf::CircleShape hard_check_box;
    std::optional<sf::Text> easy;
    std::optional<sf::Text> medium;
    std::optional<sf::Text> hard;

    sf::Texture back_button_texture;
    std::optional<sf::Sprite> back_button;

    void handleVolumeSliderInteraction(sf::Vector2f mouse_position);
    void updateVolumeBar();
    void handleDifficultyInteraction(sf::Vector2f mouse_position);
    void updateDifficultyOptions();
};