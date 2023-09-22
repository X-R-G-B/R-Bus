/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#include "raylib.h"

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
    int width;
    int height;
    float x;
    float y;
};
