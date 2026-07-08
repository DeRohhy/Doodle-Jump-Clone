#pragma once

#include "singletons/Singleton.h"

class GameConfig : public Singleton<GameConfig> {
public:
    static constexpr unsigned int SCREEN_WIDTH = 600;
    static constexpr unsigned int SCREEN_HEIGHT = 800;
    static constexpr float SPRITE_SCALE = 0.85f;
    static constexpr float GRAVITY = 980.f;
    static constexpr float CAMERA_TRIGGER_PERCENTAGE = 0.05f;
    static constexpr float CAMERA_LERP_SPEED = 15.f;
    static constexpr unsigned int CHUNK_HEIGHT = SCREEN_HEIGHT / 2;
    static constexpr float MIN_OBJ_GAP = 40.f;
    static constexpr float MAX_OBJ_GAP = 60.f;
};