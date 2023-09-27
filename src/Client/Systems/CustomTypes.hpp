/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <string>
#include "raylib.h"

namespace Types {

    struct Rect {
            float x;
            float y;
            float width;
            float height;
    };

    struct Position {
            float x;
            float y;
    };

    struct RectangleShape {
            float width;
            float height;
    };

    struct CollisionRect {
            float width;
            float height;
    };

    struct Player {
            bool isMine;
    };

    struct Text {
            std::string text;
            Color color;
            Font font;
            float fontSize;
    };

} // namespace Types
