#pragma once

#include "singletons/Singleton.h"

class GameConfig : public Singleton<GameConfig> {
public:
    static constexpr float GRAVITY = 980.f;
    static constexpr float SPRITE_SCALE = 0.85f;
};