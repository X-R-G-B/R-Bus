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
#include "Json.hpp"

namespace Systems {
    void initEnemy(JsonType enemyType);
    void windowCollision(std::size_t, std::size_t);
    void entitiesCollision(std::size_t, std::size_t);
    void deathChecker(std::size_t, std::size_t);
    void initWave(std::size_t managerId, std::size_t systemId);
    void initPlayer(JsonType playerType);
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems();
} // namespace Systems
