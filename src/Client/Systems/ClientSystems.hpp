/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <cstddef>

namespace GraphicSystems {
    void pixelRenderer(std::size_t /*unused*/);
    void spriteRenderer(std::size_t /*unused*/);
    extern constexpr std::list<std::function<void(std::size_t)>> graphicSystems;
} // namespace GraphicSystems

namespace EventsSystems {
    void playerMovement(std::size_t /*unused*/);
    extern constexpr std::list<std::function<void(std::size_t)>> eventSystems;
}
