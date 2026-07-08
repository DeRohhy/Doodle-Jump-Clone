#include "scenes/GameOverMenu.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"
#include "scenes/Game.h"
#include "scenes/MainMenu.h"
#include "scenes/SceneManager.h"
#include <memory>

GameOverMenu::GameOverMenu(SceneManager& _manager) : Scene(_manager) {
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);
    
    restart_button_texture = ResourceManager<sf::Texture>::getInstance().get(RESTART_BUTTON_PATH);
    restart_button.emplace(restart_button_texture);
    restart_button->setScale({SPRITE_SCALE, SPRITE_SCALE});

    menu_button_texture = ResourceManager<sf::Texture>::getInstance().get(MENU_BUTTON_PATH);
    menu_button.emplace(menu_button_texture);
    menu_button->setScale({SPRITE_SCALE, SPRITE_SCALE});
}

void GameOverMenu::start() {
    // Game over
    title.emplace(font);
    title->setString("Game Over");
    title->setCharacterSize(GameConfig::TITLE_FONT_SIZE);
    title->setFillColor(GameConfig::WARNING_COLOR);
    title->setStyle(sf::Text::Bold);
    const int title_x = GameConfig::SCREEN_WIDTH / 2, title_y = 120.f;
    positionText(title.value(), title_x, title_y);    

    // High Score
    high_score.emplace(font);
    high_score->setString("High Score: ");
    high_score->setCharacterSize(GameConfig::NORMAL_FONT_SIZE);
    high_score->setFillColor(GameConfig::MAIN_COLOR);
    high_score->setStyle(sf::Text::Bold);
    const int high_score_x = GameConfig::SCREEN_WIDTH / 2, high_score_y = 300.f;
    positionText(high_score.value(), high_score_x, high_score_y);

    // Score 
    score.emplace(font);
    score->setString("Score: ");
    score->setCharacterSize(GameConfig::NORMAL_FONT_SIZE);
    score->setFillColor(GameConfig::MAIN_COLOR);
    score->setStyle(sf::Text::Bold);
    const int score_x = GameConfig::SCREEN_WIDTH / 2, score_y = high_score_y + 50.f;
    positionText(score.value(), score_x, score_y);

    // Restart button
    const sf::Vector2f restart_button_bound = restart_button->getLocalBounds().size;
    restart_button->setOrigin({restart_button_bound.x / 2.f, restart_button_bound.y / 2.f});
    const int restart_button_x = GameConfig::SCREEN_WIDTH / 2, restart_button_y = 500;
    restart_button->setPosition({restart_button_x, restart_button_y});

    // Menu button
    const sf::Vector2f menu_button_bound = menu_button->getLocalBounds().size;
    menu_button->setOrigin({menu_button_bound.x / 2.f, menu_button_bound.y / 2.f});
    const int menu_button_x = GameConfig::SCREEN_WIDTH / 2, menu_button_y = restart_button_y + 100.f;
    menu_button->setPosition({menu_button_x, menu_button_y});
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

void GameOverMenu::positionText(sf::Text& text, float x, float y) {
    sf::Vector2f local_bound = text.getLocalBounds().size;
    text.setOrigin({local_bound.x / 2, local_bound.y / 2});
    text.setPosition({x, y});
}