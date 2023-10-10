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
            OtherPlayer(unsigned int id) : constId(id)
            {
            }
            unsigned int constId;
    };

    struct Missiles {
            missileTypes_e type;
    };

    struct PlayerAllies { };

    struct EnemyAllies { };

    struct Enemy {
        public:
            Enemy() : _constId(enemy_id_s {_enemyNb})
            {
                _enemyNb++;
            }
            [[nodiscard]] const enemy_id_s &getConstId() const
            {
                return _constId;
            }

        private:
            enemy_id_s _constId;
            static unsigned int _enemyNb;
    };

    struct Parallax {
            float x;
            float y;
    };

    struct Dead {
            Dead(std::size_t time = 0)
                : deathFunction(std::nullopt),
                  timeToWait(time),
                  clockId(static_cast<std::size_t>(-1)),
                  launched(false) {};
            Dead(std::optional<std::function<void(std::size_t id)>> func, std::size_t time = 0)
                : deathFunction(func),
                  timeToWait(time),
                  clockId(static_cast<std::size_t>(-1)),
                  launched(false) {};
            std::optional<std::function<void(std::size_t id)>> deathFunction;
            std::size_t timeToWait;
            std::size_t clockId;
            bool launched;
    };

} // namespace Types
