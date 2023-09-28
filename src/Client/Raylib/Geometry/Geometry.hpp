/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Geometry
*/

#pragma once

#include <cstdint>
#include "raylib.h"

namespace Raylib {

    struct Vector2 {
            Vector2(float x, float y);
            float x;
            float y;
    };

    struct Vector3 {
            Vector3(float x, float y, float z);
            float x;
            float y;
            float z;
    };

    struct Vector4 {
            Vector4(float x, float y, float z, float w);
            float x;
            float y;
            float z;
            float w;
    };

    struct Rectangle {
            Rectangle(float x, float y, float width, float height);
            float x;
            float y;
            float width;
            float height;
    };

    struct Color {
            Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
            uint8_t r;
            uint8_t g;
            uint8_t b;
            uint8_t a;
    };

    // COLOR CONSTANTS
    static const Color DarkGray   = DARKGRAY;
    static const Color Yellow     = YELLOW;
    static const Color Gold       = GOLD;
    static const Color Orange     = ORANGE;
    static const Color Pink       = PINK;
    static const Color Red        = RED;
    static const Color Maroon     = MAROON;
    static const Color Green      = GREEN;
    static const Color Lime       = LIME;
    static const Color DarkGreen  = DARKGREEN;
    static const Color SkyBlue    = SKYBLUE;
    static const Color Blue       = BLUE;
    static const Color DarkBlue   = DARKBLUE;
    static const Color Purple     = PURPLE;
    static const Color Violet     = VIOLET;
    static const Color DarkPurple = DARKPURPLE;
    static const Color Beige      = BEIGE;
    static const Color Brown      = BROWN;
    static const Color DarkBrown  = DARKBROWN;
    static const Color White      = WHITE;
    static const Color Black      = BLACK;
    static const Color Blank      = BLANK;
    static const Color Magenta    = MAGENTA;
    static const Color RayWhite   = RAYWHITE;
} // namespace Raylib
