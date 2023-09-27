/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems
*/

#pragma once

#include <cstddef>
#include <vector>
#include <functional>

namespace Systems {
    namespace EventsSystems {
        void playerMovement(std::size_t /*unused*/);
        const std::vector<std::function<void(std::size_t)>> getEventSystems();
    } // namespace EventsSystems
}
