/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <list>
#include <functional>
#include <cstddef>

namespace Systems {
    namespace GraphicSystems {
        void rectRenderer(std::size_t);
        void spriteRenderer(std::size_t /*unused*/);
        extern const std::list<std::function<void(std::size_t)>> graphicSystems;
    } // namespace GraphicSystems

    namespace EventsSystems {
        void playerMovement(std::size_t /*unused*/);
        extern const std::list<std::function<void(std::size_t)>> eventSystems;
    } // namespace EventsSystems
}
