#include "scenes/MainMenu.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameConfig.h"
#include <iostream>



MainMenu::MainMenu(SceneManager& _manager) : Scene(_manager) {
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);
}

void MainMenu::start() {
    title.emplace(font);
    title->setString("Doodle Jump");
    title->setCharacterSize(GameConfig::TITLE_FONT_SIZE);
    title->setFillColor(GameConfig::MAIN_COLOR);
    title->setStyle(sf::Text::Bold);
    const int title_x = GameConfig::SCREEN_WIDTH / 2, title_y = 120.f;
    positionText(title.value(), title_x, title_y);


    high_score.emplace(font);
    high_score->setString("High Score: ");
    high_score->setCharacterSize(GameConfig::NORMAL_FONT_SIZE);
    high_score->setFillColor(GameConfig::MAIN_COLOR);
    high_score->setStyle(sf::Text::Bold);
    const int high_score_x = GameConfig::SCREEN_WIDTH / 2, high_score_y = 300.f;
    positionText(high_score.value(), high_score_x, high_score_y);


    button_texture = ResourceManager<sf::Texture>::getInstance().get(BUTTON_PATH);
    start_button.emplace(button_texture);
    start_button->setScale({SPRITE_SCALE, SPRITE_SCALE});

    sf::Vector2f button_bound = start_button->getLocalBounds().size;
    start_button->setOrigin({button_bound.x / 2.f, button_bound.y / 2.f});
    start_button->setPosition({GameConfig::SCREEN_WIDTH / 2, GameConfig::SCREEN_HEIGHT / 2});


    tutorial.emplace(font);
    tutorial->setString("Use A / D keys to move");
    tutorial->setCharacterSize(GameConfig::NORMAL_FONT_SIZE);
    tutorial->setFillColor(GameConfig::MAIN_COLOR);
    const int tutorial_x = GameConfig::SCREEN_WIDTH / 2, tutorial_y = GameConfig::SCREEN_HEIGHT - 300;
    positionText(tutorial.value(), tutorial_x, tutorial_y);

}

void MainMenu::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse_pressed->button ==  sf::Mouse::Button::Left) {
                sf::Vector2i mouse_position = mouse_pressed->position;
                
                if (start_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    std::cout << "playing!!!\n";
                }

            }
        }
    }
}


void MainMenu::render(sf::RenderWindow& window) {  
    window.clear();
    window.draw(background_sprite.value());
    window.draw(title.value());
    window.draw(high_score.value());
    window.draw(start_button.value());
    window.draw(tutorial.value());
}

void MainMenu::positionText(sf::Text& text, float x, float y) {
    sf::Vector2f local_bound = text.getLocalBounds().size;
    text.setOrigin({local_bound.x / 2, local_bound.y / 2});
    text.setPosition({x, y});
}
