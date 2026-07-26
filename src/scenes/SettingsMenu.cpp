#include "scenes/SettingsMenu.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameSettings.h"
#include "Theme.h"
#include "GameConstants.h"

#include "scenes/SceneManager.h"
#include "scenes/MainMenu.h"


SettingsMenu::SettingsMenu(SceneManager& _manager) : Scene(_manager) {
    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    back_button_texture = ResourceManager<sf::Texture>::getInstance().get(BACK_BUTTON_PATH);
}

void SettingsMenu::start() {
    const sf::Vector2f title_position = {GameConstants::SCREEN_WIDTH / 2, 120.f};
    makeText(title, "Settings", font, Theme::FONT_TITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    title->setPosition(title_position);

    const sf::Vector2f volume_position = {GameConstants::SCREEN_WIDTH / 2, title_position.y + 100.f};
    makeText(volume, "Volume", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    volume->setPosition(volume_position);

    const sf::Vector2f volume_slider_position = {GameConstants::SCREEN_WIDTH / 2, 300.f};
    const sf::Vector2f volume_slider_start = {volume_slider_position.x - VOLUME_SLIDER_LEN / 2, volume_slider_position.y};
    const sf::Vector2f volume_slider_end = {volume_slider_position.x + VOLUME_SLIDER_LEN / 2, volume_slider_start.y};
    makeLine(volume_background, volume_slider_start, volume_slider_end, Theme::TEXT_SECONDARY, VOLUME_SLIDER_THICKNESS);

    const sf::Vector2f volume_percentage_position = {volume_slider_position.x, volume_slider_position.y + 30};
    makeText(volume_percentage, "", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    volume_percentage->setPosition(volume_percentage_position);

    updateVolumeBar();

    const sf::Vector2f difficulty_position = {GameConstants::SCREEN_WIDTH / 2, volume_slider_position.y + 100.f};
    makeText(difficulty, "Difficulty", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    difficulty->setPosition(difficulty_position);


    const float difficulty_divider = 50.f;
    const sf::Vector2f easy_position = {GameConstants::SCREEN_WIDTH / 2, difficulty_position.y + difficulty_divider};
    const sf::Vector2f medium_position = {GameConstants::SCREEN_WIDTH / 2, easy_position.y + difficulty_divider};
    const sf::Vector2f hard_position = {GameConstants::SCREEN_WIDTH / 2, medium_position.y + difficulty_divider};

    makeText(easy, "Easy", font, Theme::FONT_SUBTITLE, Theme::TEXT_SECONDARY, sf::Text::Style::Regular);
    makeText(medium, "Medium", font, Theme::FONT_SUBTITLE, Theme::TEXT_SECONDARY, sf::Text::Style::Regular);
    makeText(hard, "Hard", font, Theme::FONT_SUBTITLE, Theme::TEXT_SECONDARY, sf::Text::Style::Regular);

    easy->setPosition(easy_position);
    medium->setPosition(medium_position);
    hard->setPosition(hard_position);

    easy_check_box.setRadius(CHECK_BOX_RADIUS);
    medium_check_box.setRadius(CHECK_BOX_RADIUS);
    hard_check_box.setRadius(CHECK_BOX_RADIUS);

    const float check_label_spacing = 100.f;
    easy_check_box.setPosition({easy_position.x - check_label_spacing, easy_position.y});
    medium_check_box.setPosition({medium_position.x - check_label_spacing, medium_position.y});
    hard_check_box.setPosition({hard_position.x - check_label_spacing, hard_position.y});

    easy_check_box.setFillColor(Theme::TEXT_SECONDARY);
    medium_check_box.setFillColor(Theme::TEXT_SECONDARY);
    hard_check_box.setFillColor(Theme::TEXT_SECONDARY);

    updateDifficultyOptions();

    const sf::Vector2f back_button_position = {GameConstants::SCREEN_WIDTH / 2, GameConstants::SCREEN_HEIGHT - 100.f};
    makeButton(back_button, back_button_texture, Theme::SCALE_SMALL);
    back_button->setPosition(back_button_position);

}

void SettingsMenu::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        } 
        // allow draggin the slider while holding the LMB
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            handleVolumeSliderInteraction(mouse_position);
        }
    
        if (const auto* mouse_pressed = event->getIf<sf::Event::MouseButtonPressed>())
        {
            if (mouse_pressed->button ==  sf::Mouse::Button::Left) {
                sf::Vector2i mouse_position = mouse_pressed->position;
                
                handleDifficultyInteraction(static_cast<sf::Vector2f>(mouse_position));
                
                if (back_button->getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse_position))) {
                    manager.changeScene(std::make_unique<MainMenu>(manager));
                } 
            }
        } 
    }
}

void SettingsMenu::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.clear();
    
    window.draw(background_sprite.value());
    
    window.draw(title.value());

    window.draw(volume.value());
    window.draw(volume_background);
    window.draw(volume_bar);
    window.draw(volume_percentage.value());

    window.draw(difficulty.value());

    window.draw(easy.value());
    window.draw(medium.value());
    window.draw(hard.value());

    window.draw(easy_check_box);
    window.draw(medium_check_box);
    window.draw(hard_check_box);

    window.draw(back_button.value());
}

void SettingsMenu::handleVolumeSliderInteraction(sf::Vector2f mouse_position) {
    const sf::Vector2f slider = volume_background.getGlobalBounds().position;
    static constexpr float TOLERANCE = 7.f;
    if (mouse_position.y >= slider.y - (VOLUME_SLIDER_THICKNESS + TOLERANCE) && mouse_position.y <= slider.y + (VOLUME_SLIDER_THICKNESS + TOLERANCE) &&
        mouse_position.x >= slider.x - TOLERANCE && mouse_position.x <= slider.x + (VOLUME_SLIDER_LEN + TOLERANCE)) 
    {
        float new_volume = (mouse_position.x - slider.x) / VOLUME_SLIDER_LEN;
        new_volume = std::clamp(new_volume, 0.f, 1.f);

        constexpr float snap_threshold = 0.02f;
        if (new_volume < snap_threshold) new_volume = 0;
        else if (new_volume > 1 - snap_threshold) new_volume = 1;

        GameSettings::getInstance().setVolume(new_volume);

        updateVolumeBar();
    }
}


void SettingsMenu::updateVolumeBar() {
    const float volume = GameSettings::getInstance().getVolume();
    const sf::Vector2f bar_start = volume_background.getGlobalBounds().position;
    const sf::Vector2f knob_pos = {bar_start.x + (volume * VOLUME_SLIDER_LEN), bar_start.y};

    makeLine(volume_bar, bar_start, knob_pos, Theme::TEXT_PRIMARY, VOLUME_SLIDER_THICKNESS);

    volume_percentage->setString(std::to_string(static_cast<int>(volume * 100)) + "%");
    sf::Vector2f percentage_bounds = volume_percentage->getLocalBounds().size;
    volume_percentage->setOrigin({percentage_bounds.x / 2, percentage_bounds.y / 2});
}

void SettingsMenu::handleDifficultyInteraction(sf::Vector2f mouse_position) {
    auto& settings = GameSettings::getInstance();
    Difficulty difficulty = settings.getDifficulty();


    if (easy_check_box.getGlobalBounds().contains(mouse_position) && difficulty != Difficulty::EASY) {
        settings.setDifficulty(Difficulty::EASY);
    } else if (medium_check_box.getGlobalBounds().contains(mouse_position) && difficulty != Difficulty::MEDIUM) {
        settings.setDifficulty(Difficulty::MEDIUM);
    } else if (hard_check_box.getGlobalBounds().contains(mouse_position) && difficulty != Difficulty::HARD) {
        settings.setDifficulty(Difficulty::HARD);
    } else {
        return;
    }

    updateDifficultyOptions();
}

void SettingsMenu::updateDifficultyOptions() {
    Difficulty difficulty = GameSettings::getInstance().getDifficulty();

    easy_check_box.setFillColor(Theme::TEXT_SECONDARY);
    medium_check_box.setFillColor(Theme::TEXT_SECONDARY);
    hard_check_box.setFillColor(Theme::TEXT_SECONDARY);

    easy->setFillColor(Theme::TEXT_SECONDARY);
    medium->setFillColor(Theme::TEXT_SECONDARY);
    hard->setFillColor(Theme::TEXT_SECONDARY);

    switch (difficulty)
    {
    default:
    case Difficulty::EASY:
        easy_check_box.setFillColor(Theme::TEXT_PRIMARY);
        easy->setFillColor(Theme::TEXT_PRIMARY);
        break;
    case Difficulty::MEDIUM:
        medium_check_box.setFillColor(Theme::TEXT_PRIMARY);
        medium->setFillColor(Theme::TEXT_PRIMARY);
        break;
    case Difficulty::HARD:
        hard_check_box.setFillColor(Theme::TEXT_PRIMARY);
        hard->setFillColor(Theme::TEXT_PRIMARY);
        break;
    }
}
