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

    auto positionIt = arrPosition.begin();
    auto rectIt     = arrRect.begin();

    while (positionIt != arrPosition.end() && rectIt != arrRect.end()) {
        if (positionIt->has_value() && rectIt->has_value()) {
            Types::Position &position        = positionIt->value();
            Types::RectangleShape &rectangle = rectIt->value();

            double x = (position.x * GetScreenWidth()) / 100;
            double y = (position.y * GetScreenHeight()) / 100;

            double width  = (rectangle.width * GetScreenWidth()) / 100;
            double height = (rectangle.height * GetScreenHeight()) / 100;

            DrawRectangle(
            static_cast<int>(x), static_cast<int>(y), static_cast<int>(width),
            static_cast<int>(height), PURPLE);
        }
        positionIt++;
        rectIt++;
    }
}

void EventsSystems::playerMovement(std::size_t)
{
    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();

    for (auto &position : arrPosition) {
        if (IsKeyDown(KEY_RIGHT)) {
            position.value().x += 1;
        }
        if (IsKeyDown(KEY_LEFT)) {
            position.value().x -= 1;
        }
        if (IsKeyDown(KEY_UP)) {
            position.value().y -= 1;
        }
        if (IsKeyDown(KEY_DOWN)) {
            position.value().y += 1;
        }
    }
}
