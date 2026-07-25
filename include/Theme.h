#pragma once

#include <SFML/Graphics.hpp>

namespace Theme {
    // Font Size
    inline constexpr unsigned int FONT_TITLE = 48;
    inline constexpr unsigned int FONT_SUBTITLE = 24;

    // Colors(Text):
    inline const sf::Color TEXT_PRIMARY = sf::Color(23, 54, 93);

    // Scales(Sprite):
    inline constexpr float SCALE_SMALL = 0.35f;
    inline constexpr float SCALE_MEDIUM = 0.7f;

}