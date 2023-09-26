/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include <string>
#include "raylib.h"

namespace Types {

    struct Pixel {
            int x;
            int y;
    };

    // Size in %
    struct Sprite {
            Sprite(const std::string path, float _width, float _height)
                : sprite(LoadTexture(path.c_str())),
                  width(_width),
                  height(_height) {};
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

    struct Text {
            std::string text;
            Color color;
            Font font;
            float fontSize;
    };

} // namespace Types
