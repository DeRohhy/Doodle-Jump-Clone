#include "scenes/MainMenu.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameSettings.h"

#include "scenes/Game.h"
#include "scenes/SceneManager.h"
#include "scenes/SettingsMenu.h"

#include "Theme.h"
#include "GameConstants.h"

#include <memory>


MainMenu::MainMenu(SceneManager& _manager) : Scene(_manager) {
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);
    start_button_texture = ResourceManager<sf::Texture>::getInstance().get(START_BUTTON_PATH);
    settings_button_texture = ResourceManager<sf::Texture>::getInstance().get(SETTINGS_BUTTON_PATH);
}

void MainMenu::start() {
    const sf::Vector2f title_position = {GameConstants::SCREEN_WIDTH / 2, 120.f};
    makeText(title, "Doodle Jump", font, Theme::FONT_TITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    title->setPosition(title_position);

    const sf::Vector2f high_score_position = {GameConstants::SCREEN_WIDTH / 2, 200.f};
    const std::string high_score_text = "High Score: " + std::to_string(GameSettings::getInstance().getHighScore());
    makeText(high_score, high_score_text, font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    high_score->setPosition(high_score_position);

    const sf::Vector2f start_button_position = {GameConstants::SCREEN_WIDTH / 2, 300.f};
    makeButton(start_button, start_button_texture, Theme::SCALE_MEDIUM);
    start_button->setPosition(start_button_position);

    const sf::Vector2f settings_button_position = {GameConstants::SCREEN_WIDTH / 2, 400.f};
    makeButton(settings_button, settings_button_texture, Theme::SCALE_SMALL);
    settings_button->setPosition(settings_button_position);

    const sf::Vector2f mode_position = {GameConstants::SCREEN_WIDTH / 2, 500.f};
    makeText(mode, "Doodle Jump", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Regular);
    mode->setPosition(mode_position);


    const sf::Vector2f tutorial_move_position = {GameConstants::SCREEN_WIDTH / 2, 550.f};
    makeText(tutorial_move, "Use A / D Keys To Move", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Regular);
    tutorial_move->setPosition(tutorial_move_position);


    const sf::Vector2f tutorial_shoot_position = {GameConstants::SCREEN_WIDTH / 2, 600.f};
    makeText(tutorial_shoot, "Press LMB To Shoot", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Regular);
    tutorial_shoot->setPosition(tutorial_shoot_position);
}

void MainMenu::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } 
        if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse_pressed->button ==  sf::Mouse::Button::Left) {
                sf::Vector2i mouse_position = mouse_pressed->position;
                
                if (start_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    manager.changeScene(std::make_unique<Game>(manager));
                } else if (settings_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    manager.changeScene(std::make_unique<SettingsMenu>(manager));
                }
            }
        } 
    }
}


void MainMenu::render(sf::RenderWindow& window) {  
    window.setView(window.getDefaultView());
    window.clear();
    window.draw(background_sprite.value());
    window.draw(title.value());
    window.draw(high_score.value());
    window.draw(start_button.value());
    window.draw(settings_button.value());
    window.draw(mode.value());
    window.draw(tutorial_move.value());
    window.draw(tutorial_shoot.value());
}