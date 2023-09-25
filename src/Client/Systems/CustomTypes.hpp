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
 
    struct Pixel {
            int x;
            int y;
    };

    struct Sprite {
            Texture2D sprite;
            float width;
            float height;
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
            bool needToPlay {false};
            std::string path;
    };

    struct MusicStream {
            MusicStream(std::string musicPath)
                : music(LoadMusicStream(musicPath.c_str())),
                  path(musicPath)
            {
            }
            Music music;
            bool needToPlay {false};
            bool isPlaying {false};
            std::string path;
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

} // namespace Types
