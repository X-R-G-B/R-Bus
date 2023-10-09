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
#include "nlohmann/json.hpp"

extern "C"
{
#include "MessageTypes.h"
}

// all values are in percentage of the screen

namespace Types {

    enum MissileTypes { CLASSIC };

    struct CollisionRect {
            float width;
            float height;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionRect, width, height);
    };

    struct RectangleShape {
            float width;
            float height;
    };

    struct Position {
            float x;
            float y;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Position, x, y);
    };

    struct InitialPosition {
            float x;
            float y;
    };

    struct Damage {
            int damage;
    };

    struct Velocity {
            float speedX;
            float speedY;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Velocity, speedX, speedY);
    };

    struct Player { };

    struct OtherPlayer {
        public:
            OtherPlayer(std::size_t id) : constId(id)
            {
            }
            unsigned int constId;
    };

    struct Missiles {
            MissileTypes type;
    };

    struct PlayerAllies { };

    struct EnemyAllies { };

    struct Enemy {
        public:
            Enemy() : constId(enemy_id_s {enemyNb})
            {
                enemyNb++;
            }
            Enemy& operator=(const Types::Enemy&) = delete;
            const struct enemy_id_s constId;
            static unsigned int enemyNb;
    };

    struct Parallax { };

    struct Dead {
            Dead(std::optional<std::function<void(std::size_t id)>> func, std::size_t time = 0)
                : deathFunction(func),
                  timeToWait(time),
                  launched(false)
            {
                clockId = static_cast<std::size_t>(-1);
            };
            std::optional<std::function<void(std::size_t id)>> deathFunction;
            std::size_t timeToWait;
            std::size_t clockId;
            bool launched;
    };

} // namespace Types
