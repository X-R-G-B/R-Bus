/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include "nlohmann/json.hpp"
extern "C"
{
#include "MessageTypes.h"
}

// all values are in percentage of the screen
namespace Types {

    struct CollisionRect {
            int width;
            int height;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(CollisionRect, width, height);
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
            int speedX;
            int speedY;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Velocity, speedX, speedY);
    };

    struct Player {
            unsigned int constId;
    };

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
            Enemy(enum enemy_type_e type = enemy_type_e::CLASSIC_ENEMY) : _type(type)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                _constId = enemy_id_s {_enemyNb};
                _enemyNb++;
            }

            [[nodiscard]] const enemy_id_s &getConstId() const
            {
                std::lock_guard<std::mutex> lock(_mutex);

                return _constId;
            }

            [[nodiscard]] enum enemy_type_e getType() const
            {
                return _type;
            }

            static void setEnemyNb(unsigned int nb)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                _enemyNb = nb;
            }

        private:
            enemy_id_s _constId;
            static unsigned int _enemyNb;
            enum enemy_type_e _type;
            static std::mutex _mutex;
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
