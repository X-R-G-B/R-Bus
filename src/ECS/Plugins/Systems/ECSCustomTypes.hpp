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
#include <vector>
#include "Clock.hpp"
#include "nlohmann/json.hpp"
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

    enum PhysicsType {
        ZIGZAG = 0,
        BOUNCING
    };

    class Physics {
        public:
            inline static const std::map<std::string, PhysicsType> physicsTypeMap = {
                {"bouncing", BOUNCING},
                {"zigzag",   ZIGZAG  }
            };
            Physics(const Types::Position &originPos) : _originPos(originPos)
            {
            }

            void addPhysic(PhysicsType type)
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
                    Logger::error("Physics type not found");
                    return;
                }
                addPhysic(it->second);
            }
            std::optional<std::size_t> getClock(PhysicsType type) const
            {
                if (_physicsMap.find(type) == _physicsMap.end()) {
                    Logger::error("Physics not found");
                    throw std::runtime_error("Get clock: Physics of type " + std::to_string(type) + " not found");
                }
                return _physicsMap.at(type);
            }
            std::vector<PhysicsType> getPhysics() const
            {
                std::vector<PhysicsType> physics;

                for (const auto &pair : _physicsMap) {
                    physics.push_back(pair.first);
                }
                return physics;
            }
            bool hasPhysics(PhysicsType type) const
            {
                return _physicsMap.find(type) != _physicsMap.end();
            }
            bool hasPhysics() const
            {
                return !_physicsMap.empty();
            }
            void removePhysics(PhysicsType type)
            {
                if (_physicsMap.find(type) == _physicsMap.end()) {
                    Logger::error("Physics not found");
                    return;
                }
                _physicsMap.erase(type);
            }
            std::size_t getClockId(PhysicsType type) const
            {
                auto it = _physicsMap.find(type);
                if (it == _physicsMap.end()) {
                    Logger::error("Get clock id: Physics not found");
                    throw std::runtime_error("Physics of type " + std::to_string(type) + " not found in getClockId");
                }
                return it->second.value();
            }
            const Types::Position &getOriginPos() const
            {
                return _originPos;
            }

        private:
            // we have a map with a physic and an optional clock
            // because some physics don't need a clock
            std::unordered_map<PhysicsType, std::optional<std::size_t>> _physicsMap;
            Types::Position _originPos;
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

    struct Container {
        float minX;
        float maxX;
        float minY;
        float maxY;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(Container, minX, maxX, minY, maxY);
    };

    struct NoRemoveOutside {};

} // namespace Types
