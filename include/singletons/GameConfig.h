#pragma once

#include <SFML/Graphics.hpp>

#include "singletons/Singleton.h"

class GameConfig : public Singleton<GameConfig> {
public:
    static constexpr unsigned int SCREEN_WIDTH = 600;
    static constexpr unsigned int SCREEN_HEIGHT = 800;
    static constexpr float GRAVITY = 980.f;
    static constexpr float CAMERA_TRIGGER_PERCENTAGE = 0.3f;
    static constexpr float CAMERA_LERP_SPEED = 15.f;
    static constexpr unsigned int CHUNK_HEIGHT = SCREEN_HEIGHT / 2;
    static constexpr float MIN_OBJ_GAP = 80.f;
    static constexpr float MAX_OBJ_GAP = 150.f;
    static constexpr int TITLE_FONT_SIZE = 48;
    static constexpr int NORMAL_FONT_SIZE = 24;
    static constexpr sf::Color MAIN_COLOR = sf::Color(23, 54, 93);
    static constexpr sf::Color WARNING_COLOR = sf::Color(254, 0, 0);
};