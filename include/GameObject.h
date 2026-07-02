#pragma once

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class GameObject {
public:
    GameObject(sf::Vector2f _position, sf::Vector2f _velocity)
        : position(_position), velocity(_velocity) {}
    virtual ~GameObject() = default;

    virtual void start() {}
    virtual void update(float /*delta*/) {}
    virtual void render(sf::RenderWindow& /*window*/) {}

    sf::Vector2f getPosition() const { return position; }
    sf::Vector2f getVelocity() const { return velocity; }

    void setPosition(const sf::Vector2f& new_position) { position = new_position; };
    void setVelocity(const sf::Vector2f& new_velocity) { velocity = new_velocity; };
protected:
    sf::Vector2f position;
    sf::Vector2f velocity;
};