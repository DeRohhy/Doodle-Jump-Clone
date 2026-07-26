#include "scenes/Game.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameSettings.h"
#include "scenes/GameOverMenu.h"
#include "scenes/SceneManager.h"

#include "Theme.h"

Game::Game(SceneManager& _manager) : Scene(_manager) {
    camera = sf::View(sf::FloatRect(
        {0, 0},
        {GameConstants::SCREEN_WIDTH, GameConstants::SCREEN_HEIGHT}
    ));

    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    sf::Vector2f player_position = {GameConstants::SCREEN_WIDTH / 2, BOTTOM_Y};
    sf::Vector2f player_velocity = {GameConstants::SCREEN_WIDTH / 2, BOTTOM_Y};
    float player_fire_rate;
    switch (GameSettings::getInstance().getDifficulty())
    {
        default:
        case Difficulty::EASY:
            player_fire_rate = 0.3;
            break;
        case Difficulty::MEDIUM:
            player_fire_rate = 0.5;
            break;
        case Difficulty::HARD:
            player_fire_rate = 0.7;
            break;
    }

    player = std::make_unique<Player>(
        player_position,
        player_velocity,
        player_fire_rate
    );

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    score = 0;
}

void Game::start() {
    const sf::Vector2f score_label_position = {20.f, 20.f};
    makeText(score_label, "", font, Theme::FONT_SUBTITLE, Theme::TEXT_PRIMARY, sf::Text::Style::Bold);
    score_label->setPosition(score_label_position);

    player->start();
    // give player a starting jump
    player->handleJump();
    
    // generate 2 starting chunks
    generateChunk();
    generateChunk();
}

void Game::update(float delta) {
    int new_score = BOTTOM_Y - static_cast<int>(player->getPosition().y);
    if (new_score > score)
    {
        score = new_score;
        score_label->setString("Score: " + std::to_string(score));
    }

    player->update(delta);
    for (const auto& chunk: chunks) {
        chunk->update(delta);
    }

    checkChunkGeneration();
    handleChunkDeletion();
    updateCameraPosition();

    const float camera_bottom_y = camera.getCenter().y + (GameConstants::SCREEN_HEIGHT / 2.f);
    if (player->getPosition().y - (player->getBodyBounds().size.y + player->getFeetBounds().size.y) > camera_bottom_y) {
        handleGameOver();
    }
    for (const auto& chunk: chunks) {
        if (chunk->isGameOver()) {
            handleGameOver();
        }
    }
}

void Game::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }   
}

void Game::render(sf::RenderWindow& window) {
    window.setView(window.getDefaultView());
    window.draw(background_sprite.value());

    
    window.setView(camera);
    for (const auto& chunk: chunks) {
        chunk->render(window);
    }
    player->render(window);

    window.setView(window.getDefaultView());
    window.draw(score_label.value());
}

void Game::updateCameraPosition() {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y;

    if (target_y >= threshold) {
        return;
    }

    const float new_y = camera_center_y + (target_y - threshold);

    camera.setCenter({camera.getCenter().x, new_y});
}

void Game::checkChunkGeneration() {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y;
    
    static const int MAX_CHUNKS_ALLOWED = 3;
    if (target_y <= threshold && chunks.size() < MAX_CHUNKS_ALLOWED) {
        generateChunk();    
    } 
}

void Game::handleChunkDeletion() {
    const float camera_bottom_y = camera.getCenter().y + (GameConstants::SCREEN_HEIGHT / 2.f);

    while (!chunks.empty() && chunks.back()->getPosition().y >= camera_bottom_y) {
        chunks.pop_back();
    }
}

void Game::generateChunk() {
    const float chunk_position_y = 
        chunks.empty() ? 0 : 
                         chunks.front()->get_highest_platform_y() - GameConstants::CHUNK_HEIGHT;
    
    std::unique_ptr<Chunk> new_chunk = std::make_unique<Chunk>(
        sf::Vector2f({0, chunk_position_y}),
        player.get(),
        &camera
    );
    new_chunk->start();
    
    chunks.push_front(std::move(new_chunk));    
}

void Game::handleGameOver() {
    GameSettings::getInstance().setLastScore(score);
    GameSettings::getInstance().setHighScore(score);
    manager.changeScene(std::make_unique<GameOverMenu>(manager));
}