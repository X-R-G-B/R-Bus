/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace EventsSystems {
        void playerMovement(std::size_t /*unused*/, std::size_t /*unused*/);
        void changeScene(std::size_t /*unused*/, std::size_t /*unused*/);
        void shootBullet(std::size_t, std::size_t);
        std::vector<std::function<void(std::size_t, std::size_t)>>
        getEventSystems();
    } // namespace EventsSystems
} // namespace Systems
