#pragma once

#include <SFML/Graphics.hpp>

class SceneManager;

class Scene {
public:
    explicit Scene(SceneManager& _manager) : manager(_manager) {}
    virtual ~Scene() = default;

    virtual void start() = 0;
    virtual void update(float delta) = 0;
    virtual void handleEvents(sf::RenderWindow& window) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
protected:
    SceneManager& manager;
};