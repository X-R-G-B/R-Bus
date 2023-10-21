#pragma once

#include "ECSCustomTypes.hpp"
#include "GameCustomTypes.hpp"
extern "C"
{
#include "MessageTypes.h"
}

enum class AlliesType {
    PLAYERS,
    ENEMIES
};

namespace Systems {
    void initEnemy(
            enemy_type_e enemyType,
            Types::Position position,
            bool setId                  = false,
            struct ::enemy_id_s enemyId = {0});
    void initWave(std::size_t managerId, std::size_t systemId);
    void createMissile(Types::Position &pos, Types::Missiles &typeOfMissile);
    void initPlayer(unsigned int constId, bool otherPlayer = false);
    std::vector<std::function<void(std::size_t, std::size_t)>> getGameSystems();
}
