/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <string>
#include "raylib.h"
#include "ECSCustomTypes.hpp"

namespace Types {

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

    struct Text {
            std::string text;
            Color color;
            Font font;
            float fontSize;
    };

} // namespace Types
