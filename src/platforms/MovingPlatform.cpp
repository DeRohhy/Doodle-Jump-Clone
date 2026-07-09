#include "platforms/MovingPlatform.h"
#include "singletons/GameConfig.h"

void MovingPlatform::update(float delta) {
    position.x += speed * delta;

    static const float SIDE_MARGIN = 50.f;
    if ((position.x >= GameConfig::SCREEN_WIDTH - SIDE_MARGIN && speed) > 0 ||
        (position.x <= SIDE_MARGIN && speed < 0)) {
        speed *= -1;
    }

    platform_sprite->setPosition(position);
}