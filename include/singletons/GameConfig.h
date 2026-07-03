#pragma once

#include "singletons/Singleton.h"

class GameConfig : public Singleton<GameConfig> {
public:
    static constexpr float GRAVITY = 980.f;

};