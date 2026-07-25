#include "scenes/SettingsMenu.h"

#include "singletons/ResourceManager.h"


SettingsMenu::SettingsMenu(SceneManager& _manager) : Scene(_manager) {
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

}

void SettingsMenu::start() {

}


void SettingsMenu::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void SettingsMenu::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.clear();
    window.draw(background_sprite.value());
}