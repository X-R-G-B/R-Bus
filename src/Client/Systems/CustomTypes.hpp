/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include <iostream>
#include <string>
#include "raylib.h"

namespace Types {
    struct Pixel {
            int x;
            int y;
    };

    struct Sprite {
            Texture2D sprite;
    };

    struct Position {
            float x;
            float y;
    };

    struct Rect {
            float width;
            float height;
            float x;
            float y;
    };

    struct SoundEffect {
            SoundEffect(std::string soundPath)
                : sound(LoadSound(soundPath.c_str())),
                  path(soundPath)
            {
            }
            Sound sound;
            bool needToPlay{false};
            std::string path;
    };

} // namespace Types
