/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once
#include <cstddef>

namespace GraphicSystems {
    void rectRenderer(std::size_t);
    void spriteRenderer(std::size_t);
    void textRenderer(std::size_t);
} // namespace GraphicSystems

namespace EventsSystems {
    void playerMovement(std::size_t /*unused*/);
}
