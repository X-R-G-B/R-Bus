/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** DeathSystems
*/

#pragma once

#include <vector>
#include <functional>

namespace Systems {
    namespace DeathSystems {
        std::vector<std::function<void(std::size_t, std::size_t)>> getDeathSystems();
        void setEntityDeathFunction(std::size_t /*unused*/, std::size_t /*unused*/);
    } // namespace DeathSystems
} // namespace Systems
