/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <Json.hpp>
#include <any>
#include <cstddef>
#include <functional>
#include <mutex>
#include <optional>
#include <vector>
#include "Clock.hpp"
#include "nlohmann/json.hpp"
extern "C"
{
#include "MessageTypes.h"
}
#include "Registry.hpp"

#include <iostream>

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

    enum physicsType_e {
        ZIGZAG = 0,
        BOUNCING
    };

    // These structs are used to store physics data

    struct Zigzag {
            Zigzag(const Types::Position &originPosition)
                : clockId(Registry::getInstance().getClock().create(true)),
                  originPos(originPosition)
            {
            }
            std::size_t clockId;
            Types::Position originPos;
            float period     = 400.0F;
            float amplitude  = 10.0F;
            float maxScreenY = 100.0F;
            float minScreenY = 0.0F;
    };

    struct Bouncing {
            Bouncing(const Types::Position &originPosition) : originPos(originPosition)
            {
            }
            Types::Position originPos;
    };

    class Physics {
        public:
            inline static const std::map<std::string, physicsType_e> physicsTypeMap = {
                {"bouncing", BOUNCING},
                {"zigzag",   ZIGZAG  }
            };
            Physics();

            void addPhysic(nlohmann::json &jsonObject, const Types::Position &originPos);

            void removePhysic(physicsType_e type);
            void removePhysic(std::string type);
            void removePhysics();
            std::vector<physicsType_e> getPhysics() const;
            bool hasPhysics(physicsType_e type) const;
            bool hasPhysics() const;

            template <typename T>
            T &getPhysicData(physicsType_e type)
            {
                return std::any_cast<T &>(_physicsMap[type]);
            }

        private:
            void initBounce(nlohmann::json &jsonObject, const Types::Position &originPos);
            void initZigzag(nlohmann::json &jsonObject, const Types::Position &originPos);
            std::unordered_map<physicsType_e, std::any> _physicsMap;
    };

    struct PlayerAllies { };

    struct EnemyAllies { };

    struct EnnemyAttack {
            missileTypes_e missileType      = missileTypes_e::CLASSIC;
            Types::Position launchDirection = {0, 0};
            std::size_t numberOfMissiles    = 1;
    };

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

            EnnemyAttack &getAttack()
            {
                return attack;
            }

        private:
            static unsigned int _enemyNb;
            static std::mutex _mutex;
            EnnemyAttack attack;
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
