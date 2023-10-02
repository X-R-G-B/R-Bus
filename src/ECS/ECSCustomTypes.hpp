/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

// all values are in percentage of the screen

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

    struct Dammage {
            int dammage;
    };

    struct Velocity {
        float speedX;
        float speedY;
    };

    struct Player { };

    struct Missiles {
        // maybe enum better ?
        std::string type;
    };

} // namespace Types
