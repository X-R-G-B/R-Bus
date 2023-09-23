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

void GraphicSystems::RectRenderer(std::size_t)
{
    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();
    Registry::components<Types::RectangleShape> arrRect =
    Registry::getInstance().getComponents<Types::RectangleShape>();

    Types::Position position = {0, 0};
    for (auto &pos : arrPosition) {
        position = pos;
    }

    double x = (position.x * GetScreenWidth()) / 100;
    double y = (position.y * GetScreenHeight()) / 100;

    for (auto begin = arrRect.begin(); begin != arrRect.end(); begin++) {
        for (int i = 0; i < (begin->width * GetScreenWidth()) / 100; i++) {
            for (int j = 0; j < (begin->width * GetScreenHeight()) / 100; j++) {
                DrawPixel(
                static_cast<int>(x) + i, static_cast<int>(y) + j, PURPLE);
            }
        }
    }
}

void EventsSystems::playerMovement(std::size_t)
{
    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();

    for (auto &position : arrPosition) {
        if (IsKeyDown(KEY_RIGHT)) {
            position.x += 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            position.x -= 1;
        }
        if (IsKeyDown(KEY_UP)) {
            position.y -= 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            position.y += 1;
        }
    }
}
