#include "Bullet.h"

void Bullet::start() {
    bullet_shape.setRadius(BULLET_RADIUS);
    bullet_shape.setFillColor(sf::Color::Black);
    sf::Vector2f bullet_bounds = bullet_shape.getLocalBounds().size;
    bullet_shape.setOrigin({bullet_bounds.x / 2, bullet_bounds.y / 2});
}

void Bullet::update(float delta) {
    position += velocity * delta;
    bullet_shape.setPosition(position);
}

void Bullet::render(sf::RenderWindow& window) {
    window.draw(bullet_shape);
}
