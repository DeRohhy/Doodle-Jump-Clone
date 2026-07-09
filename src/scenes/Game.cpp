#include "scenes/Game.h"

#include "singletons/ResourceManager.h"
#include "singletons/GameSettings.h"
#include "scenes/GameOverMenu.h"
#include "scenes/SceneManager.h"

Game::Game(SceneManager& _manager) : Scene(_manager) {
    camera = sf::View(sf::FloatRect(
        {0, 0},
        {GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT}
    ));

    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    
    player = std::make_unique<Player>(
        sf::Vector2f{GameConfig::SCREEN_WIDTH / 2, BOTTOM_Y},
        sf::Vector2f()
    );

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    score = 0;
}

void Game::start() {
    score_label.emplace(font);
    score_label->setCharacterSize(GameConfig::NORMAL_FONT_SIZE);
    score_label->setFillColor(GameConfig::MAIN_COLOR);
    score_label->setStyle(sf::Text::Bold);
    const int score_label_x = 20, score_label_y = 20.f;
    score_label->setPosition({score_label_x, score_label_y});

    player->start();

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
    lerpCameraPosition(delta);

    const float camera_bottom_y = camera.getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);
    if (player->getPosition().y > camera_bottom_y) {
        GameSettings::getInstance().setLastScore(score);
        GameSettings::getInstance().setHighScore(score);
        manager.changeScene(std::make_unique<GameOverMenu>(manager));
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

void Game::lerpCameraPosition(float delta) {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y 
                            - GameConfig::CAMERA_TRIGGER_PERCENTAGE * (GameConfig::SCREEN_HEIGHT / 2.f);

    if (target_y >= threshold) {
        return;
    }

    // framerate-independent lerp
    const float blend = 1.f - std::exp(-GameConfig::CAMERA_LERP_SPEED * delta);
    const float new_y = camera_center_y + (target_y - threshold) * blend;

    camera.setCenter({camera.getCenter().x, new_y});
}

void Game::checkChunkGeneration() {
    const float target_y = player->getPosition().y;
    const float camera_center_y = camera.getCenter().y;

    const float threshold = camera_center_y 
                            - GameConfig::CAMERA_TRIGGER_PERCENTAGE * (GameConfig::SCREEN_HEIGHT / 2.f);
    
    static const int MAX_CHUNKS_ALLOWED = 3;
    if (target_y <+ threshold || chunks.size() < MAX_CHUNKS_ALLOWED) {
        generateChunk();    
    } 
}

void Game::handleChunkDeletion() {
    const float camera_bottom_y = camera.getCenter().y + (GameConfig::SCREEN_HEIGHT / 2.f);

    while (!chunks.empty() && chunks.back()->getPosition().y >= camera_bottom_y) {
        chunks.pop_back();
    }
}

void Game::generateChunk() {
    const float chunk_position_y = 
        chunks.empty() ? GameConfig::SCREEN_HEIGHT / 2.f : 
                         chunks.front()->getPosition().y - GameConfig::CHUNK_HEIGHT;
    
    std::unique_ptr<Chunk> new_chunk = std::make_unique<Chunk>(
        sf::Vector2f({0, chunk_position_y}),
        player.get(),
        &camera
    );
    new_chunk->start();
    
    chunks.push_front(std::move(new_chunk));    
}