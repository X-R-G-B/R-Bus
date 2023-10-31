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
#include "Clock.hpp"
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
            Physics(const Types::Position &originPos);

            void addPhysic(physicsType_e type);
            void addPhysic(std::string type);
            std::optional<std::size_t> getClock(physicsType_e type) const;
            std::vector<physicsType_e> getPhysics() const;
            bool hasPhysics(physicsType_e type) const;
            bool hasPhysics() const;
            void removePhysics(physicsType_e type);
            std::size_t getClockId(physicsType_e type) const;
            const Types::Position &getOriginPos() const;

        private:
            // we have a map with a physic and an optional clock
            // because some physics don't need a clock
            std::unordered_map<physicsType_e, std::optional<std::size_t>> _physicsMap;
            Types::Position _originPos;
    };

    struct PlayerAllies { };

    struct EnemyAllies { };

    class WaveInfos {
        public:
            WaveInfos()
                : _clockId(Registry::getInstance().getClock().create(false)), _isFistWaveStarted(false)
            {
            }

            static WaveInfos &getInstance()
			{
				static WaveInfos instance;
				return instance;
			}

			WaveInfos(const WaveInfos &) = delete;
			WaveInfos(const WaveInfos &&) = delete;
			WaveInfos &operator=(const WaveInfos &) = delete;
			WaveInfos &operator=(const WaveInfos &&) = delete;

            void setWaveId(unsigned int id)
            {
                _waveId = id;
            }

            unsigned int getWaveId()
            {
                return _waveId;
            }

            void addEnemy(const nlohmann::json &data, std::size_t msBeforeNext)
            {
                _remainingEnemies.emplace_back(data, msBeforeNext);
            }

            const std::vector<std::pair<nlohmann::json, std::size_t>> &getRemainingEnemies() const
            {
                return _remainingEnemies;
            }

            bool isEnemyRemaining() const
            {
                return !_remainingEnemies.empty();
            }

            std::size_t getClockId() const
            {
                return _clockId;
            }

            void removeFirstEnemy()
            {
                if (_isFistWaveStarted == false) {
                    _isFistWaveStarted = true;
                }
                _remainingEnemies.erase(_remainingEnemies.begin());
            }

            void setFirstWaveStarted(bool value)
            {
                _isFistWaveStarted = value;
            }

            bool isFirstWaveStarted() const
            {
                return _isFistWaveStarted;
            }

        private:
            std::vector<std::pair<nlohmann::json, std::size_t>> _remainingEnemies;
            unsigned int _waveId;
            std::size_t _clockId;
            bool _isFistWaveStarted;
    };

    struct Enemy {
        public:
            Enemy(enum enemy_type_e _type) : type(_type)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                constId = enemy_id_s {_enemyNb};
                _enemyNb++;
            }

            Enemy(enum enemy_type_e _type, enemy_id_s enemyId) : type(_type), constId(enemyId)
            {
                std::lock_guard<std::mutex> lock(_mutex);
            }

            Enemy(struct enemy_id_s _constId, enum enemy_type_e _type)
                : constId(_constId),
                  type(_type)
            {
            }

            [[nodiscard]] enemy_id_s getConstId() const
            {
                return constId;
            }

            static bool isEnemyAlive()
            {
                Registry &registry           = Registry::getInstance();
                std::vector<std::size_t> ids = registry.getEntitiesByComponents({typeid(Types::Enemy)});
                return ids.empty() == false;
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
