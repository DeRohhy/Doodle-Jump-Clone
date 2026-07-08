#include "scenes/Game.h"

#include "singletons/GameConfig.h"
#include "singletons/ResourceManager.h"

Game::Game(SceneManager& _manager) : Scene(_manager) {
    camera = sf::View(sf::FloatRect(
        {0, 0},
        {GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT}
    ));

    background_texture = ResourceManager<sf::Texture>::getInstance().get(BACKGROUND_PATH);
    background_sprite.emplace(background_texture);

    font = ResourceManager<sf::Font>::getInstance().get(FONT_PATH);

    player = std::make_unique<Player>(
        sf::Vector2f{GameConfig::SCREEN_WIDTH / 2, GameConfig::SCREEN_HEIGHT / 2},
        sf::Vector2f()
    );
}

void Game::start() {
    player->start();

    player->handleJump();
    
    // generate 2 starting chunks
    generateChunk();
    generateChunk();
}

void Game::update(float delta) {
    player->update(delta);
    for (const auto& chunk: chunks) {
        chunk->update(delta);
    }

    checkChunkGeneration();
    handleChunkDeletion();
    lerpCameraPosition(delta);
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
    if (background_sprite.has_value())
        window.draw(background_sprite.value());
    
    window.setView(camera);
    for (const auto& chunk: chunks) {
        chunk->render(window);
    }
    player->render(window);
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
        chunks.back().release();
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