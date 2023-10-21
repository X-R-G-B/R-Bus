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
}
