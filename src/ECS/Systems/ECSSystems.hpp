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
    void windowCollision(std::size_t, std::size_t);
    bool checkAllies(std::size_t fstId, std::size_t scdId);
    void entitiesCollision(std::size_t, std::size_t);
    void deathChecker(std::size_t, std::size_t);
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems();
} // namespace Systems
