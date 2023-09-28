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
} // namespace Raylib
