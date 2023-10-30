/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include "ECSCustomTypes.hpp"
#include "Raylib.hpp"

namespace Types {
    struct Rect {
        float x;
        float y;
        float width;
        float height;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Rect, x, y, width, height);
    };

    struct Parallax {
        float x;
        float y;
    };

    struct RectangleShape {
        float width;
        float height;
    };

    struct FontSize {
        float fsz;
    };

    struct Origin {
        float x;
        float y;
    };

    struct Rotation {
        float rotate;
    };

    struct Scale {
        float size;
    };

    struct Color {
            Raylib::Color color;
    };
} // namespace Types

#include "AnimRect.hpp"
