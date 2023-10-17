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
#include "Registry.hpp"

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

            Position &operator+=(const Position &pos)
            {
                x += pos.x;
                y += pos.y;
                return (*this);
            }
    };

    struct Damage {
            int damage;
    };

    struct Velocity {
            int speedX;
            int speedY;

            NLOHMANN_DEFINE_TYPE_INTRUSIVE(Velocity, speedX, speedY);
    };

    struct SpriteDatas {
            SpriteDatas(
                const std::string &fileName,
                float width,
                float height,
                enum LayerType layer,
                std::size_t layerSide)
                : fileName(fileName),
                  width(width),
                  height(height),
                  layer(layer),
                  layerSide(layerSide)
            {
            }
            std::string fileName;
            float width;
            float height;
            std::size_t id;
            enum LayerType layer;
            size_t layerSide;
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
            Enemy(enum enemy_type_e _type = enemy_type_e::CLASSIC_ENEMY) : type(_type)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                constId = enemy_id_s {_enemyNb};
                _enemyNb++;
            }

            Enemy(struct enemy_id_s _constId, enum enemy_type_e _type = enemy_type_e::CLASSIC_ENEMY)
                : constId(_constId),
                  type(_type)
            {
            }

            [[nodiscard]] enemy_id_s getConstId() const
            {
                return constId;
            }

            static void setEnemyNb(unsigned int nb)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                _enemyNb = nb;
            }

            static unsigned int getEnemyNb()
            {
                std::lock_guard<std::mutex> lock(_mutex);

                return _enemyNb;
            }

            enemy_id_s constId;
            enum enemy_type_e type;

        private:
            static unsigned int _enemyNb;
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

    struct Boss { };

} // namespace Types
