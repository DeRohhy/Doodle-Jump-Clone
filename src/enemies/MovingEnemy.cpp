#include "enemies/MovingEnemy.h"

#include "GameConstants.h"
#include "Theme.h"

void MovingEnemy::update(float delta) {
    position.x += horizontal_speed * delta;

    if ((position.x >= GameConstants::SCREEN_WIDTH - SIDE_MARGIN && horizontal_speed > 0) ||
        (position.x <= SIDE_MARGIN && horizontal_speed < 0)) {
        horizontal_speed *= -1;
    }

    if (horizontal_speed > 0) {
        enemy_sprite->setScale({Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});
    } else {
        enemy_sprite->setScale({-Theme::SCALE_MEDIUM, Theme::SCALE_MEDIUM});
    }

    enemy_sprite->setPosition(position);
}