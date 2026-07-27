#include "scenes/GameOverMenu.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameSettings.h"

#include "scenes/Game.h"
#include "scenes/MainMenu.h"
#include "scenes/SceneManager.h"

#include "Theme.h"
#include "GameConstants.h"

#include <memory>

GameOverMenu::GameOverMenu(SceneManager& _manager) : Scene(_manager) {
    game_over_buffer = ResourceManager<sf::SoundBuffer>::getInstance().get(GAME_OVER_SOUND_PATH);
    game_over_sound.emplace(game_over_buffer);
    
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);
    
    restart_button_texture = ResourceManager<sf::Texture>::getInstance().get(RESTART_BUTTON_PATH);
    menu_button_texture = ResourceManager<sf::Texture>::getInstance().get(MENU_BUTTON_PATH);
}

void GameOverMenu::start() {
    game_over_sound->play();

    const sf::Vector2f title_position = {GameConstants::SCREEN_WIDTH / 2, 120.f};
    makeText(title, "Game Over", font, Theme::FONT_TITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    title->setPosition(title_position);

    const sf::Vector2f high_score_position = {GameConstants::SCREEN_WIDTH / 2, 300.f};
    const int high_score_amount = GameSettings::getInstance().getHighScore();
    makeText(high_score, "High Score: " + std::to_string(high_score_amount), font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Bold);
    high_score->setPosition(high_score_position);

    const sf::Vector2f score_position = {GameConstants::SCREEN_WIDTH / 2, high_score_position.y + 50.f};
    const int score_amount = GameSettings::getInstance().getLastScore();
    makeText(score, "Your Score: " + std::to_string(score_amount), font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Bold);
    score->setPosition(score_position);

    const sf::Vector2f restart_button_position = {GameConstants::SCREEN_WIDTH / 2, 500.f};
    makeButton(restart_button, restart_button_texture, Theme::SCALE_MEDIUM);
    restart_button->setPosition(restart_button_position);

    const sf::Vector2f menu_button_position = {GameConstants::SCREEN_WIDTH / 2, restart_button_position.y + 100.f};
    makeButton(menu_button, menu_button_texture, Theme::SCALE_MEDIUM);
    menu_button->setPosition(menu_button_position);
}

void GameOverMenu::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } else if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse_pressed->button ==  sf::Mouse::Button::Left) {
                sf::Vector2i mouse_position = mouse_pressed->position;
                
                if (restart_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    manager.changeScene(std::make_unique<Game>(manager));
                } else if (menu_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    manager.changeScene(std::make_unique<MainMenu>(manager));
                }
            }
        }
    }
}

void GameOverMenu::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.clear();
    window.draw(background_sprite.value());
    window.draw(title.value());
    window.draw(high_score.value());
    window.draw(score.value());
    window.draw(restart_button.value());
    window.draw(menu_button.value());
}