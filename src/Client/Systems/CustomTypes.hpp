/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include "raylib.h"

namespace Types {

    struct Pixel {
            int x;
            int y;
    };

    struct Sprite {
            Texture2D sprite;
            float width;
            float height;
    };

    struct Rect {
                float x;
            float y;
            float width;
            float height;
    };

    struct RectangleShape {
            float width;
            float height;
    };

    struct CollisionRect {
            float width;
            float height;
    };

    struct Position {
            float x;
            float y;
    };

    struct Player {
            bool isMine;
    };

} // namespace Types
