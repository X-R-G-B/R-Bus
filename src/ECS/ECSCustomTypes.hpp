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
#include <vector>
#include "clock.hpp"
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
            int x;
            int y;

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

    struct SpriteDatas {
            SpriteDatas(
                const std::string &fileName,
                int width,
                int height,
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
            int width;
            int height;
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

    class Physics {
        public:
            inline static const std::map<std::string, physicsType_e> physicsTypeMap = {
                {"bouncing", BOUNCING},
                {"zigzag",   ZIGZAG  }
            };

            void addPhysic(physicsType_e type)
            {
                if (_physicsMap.find(type) != _physicsMap.end()) {
                    Logger::error("Physics already added");
                    return;
                }
                if (type == ZIGZAG) {
                    _physicsMap[type] = Registry::getInstance().getClock().create(true);
                } else {
                    _physicsMap[type] = std::nullopt;
                }
            }

            void addPhysic(std::string type)
            {
                auto it = physicsTypeMap.find(type);
                if (it == physicsTypeMap.end()) {
                    Logger::error("Physics not found");
                    return;
                }
                addPhysic(it->second);
            }

            std::optional<std::size_t> getClock(physicsType_e type) const
            {
                if (_physicsMap.find(type) == _physicsMap.end()) {
                    Logger::error("Physics not found");
                    return std::nullopt;
                }
                return _physicsMap.at(type);
            }

            std::vector<physicsType_e> getPhysics() const
            {
                std::vector<physicsType_e> physics;

                for (auto &pair : _physicsMap) {
                    physics.push_back(pair.first);
                }
                return physics;
            }

            bool hasPhysics(physicsType_e type) const
            {
                return _physicsMap.find(type) != _physicsMap.end();
            }

            bool hasPhysics() const
            {
                return !_physicsMap.empty();
            }

            void removePhysics(physicsType_e type)
            {
                if (_physicsMap.find(type) == _physicsMap.end()) {
                    Logger::error("Physics not found");
                    return;
                }
                _physicsMap.erase(type);
            }

            std::size_t getClockId(physicsType_e type) const
            {
                auto it = _physicsMap.find(type);
                if (it == _physicsMap.end()) {
                    Logger::error("Physics not found");
                    throw std::runtime_error("Physics not found");
                }
                return it->second.value();
            }

        private:
            // we have a map with a physic and an optional clock
            // because some physics don't need a clock
            std::unordered_map<physicsType_e, std::optional<std::size_t>> _physicsMap;
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
