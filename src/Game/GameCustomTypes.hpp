/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <mutex>
#include "MessageTypes.h"

namespace Types {
    struct Boss { };

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
        public:
            Missiles(enum missileTypes_e _type = CLASSIC) : type(_type), constId(0)
            {
            }

            static void setMissileNb(unsigned int nb)
            {
                std::lock_guard<std::mutex> lock(_mutex);

                _missileNb = nb;
            }

            static unsigned int getMissileNb()
            {
                std::lock_guard<std::mutex> lock(_mutex);
                return _missileNb;
            }

            missileTypes_e type;
            unsigned int constId;

        private:
            static unsigned int _missileNb;
            static std::mutex _mutex;
    };

    struct PlayerAllies { };

    struct EnemyAllies { };

    struct EnemyAttack {
            bool isAttacking                = true;
            missileTypes_e missileType      = missileTypes_e::CLASSIC;
            Types::Position launchDirection = {-1, 0};
            std::size_t numberOfMissiles    = 1;
            std::size_t clockId             = Registry::getInstance().getClock().create(false);
            float msBetweenMissiles         = 1000.0F;
            float missileSpawnOffset        = 10.0F;
            float bulletSpeed               = 20.0F;
            std::string emitterId;
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

            Enemy(struct enemy_id_s _constId, enum enemy_type_e _type) : type(_type), constId(_constId)
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

            enum enemy_type_e type;
            enemy_id_s constId;

            EnemyAttack &getAttack()
            {
                return attack;
            }

        private:
            static unsigned int _enemyNb;
            static std::mutex _mutex;
            EnemyAttack attack;
    };

    class WaveInfos {
        public:
            WaveInfos()
                : _clockId(Registry::getInstance().getClock().create(false)),
                  _isFirstEnemyCreated(false),
                  _waitingForNextWave(false)
            {
            }

            static WaveInfos &getInstance()
            {
                static WaveInfos instance;
                return instance;
            }

            WaveInfos(const WaveInfos &)             = delete;
            WaveInfos(const WaveInfos &&)            = delete;
            WaveInfos &operator=(const WaveInfos &)  = delete;
            WaveInfos &operator=(const WaveInfos &&) = delete;

            void setWaveId(unsigned int id)
            {
                _waveId = id;
            }

            unsigned int getWaveId()
            {
                return _waveId;
            }

            void addEnemy(const nlohmann::json &data, std::size_t msBeforeSpwan)
            {
                _remainingEnemies.emplace_back(data, msBeforeSpwan);
            }

            const std::vector<std::pair<nlohmann::json, std::size_t>> &getRemainingEnemies() const
            {
                return _remainingEnemies;
            }

            std::size_t getEnemiesRemaining() const
            {
                return _remainingEnemies.size();
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
                if (_isFirstEnemyCreated == false) {
                    _isFirstEnemyCreated = true;
                }
                _remainingEnemies.erase(_remainingEnemies.begin());
            }

            void setFirstEnemyCreated(bool value)
            {
                _isFirstEnemyCreated = value;
            }

            bool isFirstEnemyCreated() const
            {
                return _isFirstEnemyCreated;
            }

            bool getWaitingForNextWave() const
            {
                return _waitingForNextWave;
            }

            void setWaitingForNextWave(bool value);

            void prepareNextWave();

        private:
            std::vector<std::pair<nlohmann::json, std::size_t>> _remainingEnemies;
            unsigned int _waveId;
            std::size_t _clockId;
            bool _isFirstEnemyCreated;
            bool _waitingForNextWave;
    };
} // namespace Types
