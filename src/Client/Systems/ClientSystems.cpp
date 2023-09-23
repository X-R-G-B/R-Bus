/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "ClientSystems.hpp"
#include <iostream>
#include "raylib.h"
#include "CustomTypes.hpp"
#include "Registry.hpp"

void GraphicSystems::pixelRenderer(std::size_t)
{
    Registry::components<Pixel> arrPixel =
    Registry::getInstance().getComponents<Pixel>();
    for (auto begin = arrPixel.begin(); begin != arrPixel.end(); begin++) {
        if (!begin->has_value()) {
            continue;
        }
        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50; j++) {
                DrawPixel(begin->value().x + i, begin->value().y + j, PURPLE);
            }
        }
    }
}

void EventsSystems::playerMovement(std::size_t)
{
    Registry::components<Pixel> arrPixel =
    Registry::getInstance().getComponents<Pixel>();

    for (auto &pixel : arrPixel) {
        if (!pixel.has_value()) {
            continue;
        }
        if (IsKeyDown(KEY_RIGHT))
            pixel.value().x += 1;
        if (IsKeyDown(KEY_LEFT))
            pixel.value().x -= 1;
        if (IsKeyDown(KEY_UP))
            pixel.value().y -= 1;
        if (IsKeyDown(KEY_DOWN))
            pixel.value().y += 1;
    }
}
