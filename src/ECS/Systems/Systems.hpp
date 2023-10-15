/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include "ECSCustomTypes.hpp"
#include "Json.hpp"
#include "MessageTypes.h"

namespace Systems {
    void initEnemy(JsonType enemyType, bool setId = false, struct ::enemy_id_s enemyId = {0});
    void windowCollision(std::size_t, std::size_t);
    void entitiesCollision(std::size_t, std::size_t);
    void deathChecker(std::size_t, std::size_t);
    void initWave(std::size_t managerId, std::size_t systemId);
    void createMissile(Types::Position &pos, Types::Missiles &typeOfMissile);
    void initPlayer(JsonType playerType, unsigned int constId, bool otherPlayer = false);
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems();
} // namespace Systems
