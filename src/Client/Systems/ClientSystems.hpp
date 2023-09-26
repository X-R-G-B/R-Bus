/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicSystems {
        void rectRenderer(std::size_t /*unused*/);
        void spriteRenderer(std::size_t /*unused*/);
        void textRenderer(std::size_t /*unused*/);
        extern const std::vector<std::function<void(std::size_t)>>
            graphicSystems;
    } // namespace GraphicSystems

    namespace EventsSystems {
        void playerMovement(std::size_t /*unused*/);
        extern const std::vector<std::function<void(std::size_t)>> eventSystems;
    } // namespace EventsSystems
} // namespace Systems
