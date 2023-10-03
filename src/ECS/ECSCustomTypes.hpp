/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <cstddef>
#include <functional>
#include <optional>

namespace Types {

    struct CollisionRect {
            float width;
            float height;
    };

    struct RectangleShape {
            float width;
            float height;
    };

    struct Position {
            float x;
            float y;
    };

    struct Health {
            int hp;
    };

    struct Damage {
            int damage;
    };

    struct Player { };

    struct Enemy { };

    struct Dead {
            std::optional<std::function<void(std::size_t id)>> deathFunction;
    };

} // namespace Types
