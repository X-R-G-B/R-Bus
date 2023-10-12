/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SpriteSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicSystems {
        void rectIncrementation(std::size_t /*unused*/, std::size_t /*unused*/);
        void rectRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        void spriteRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        std::vector<std::function<void(std::size_t, std::size_t)>> getSpriteSystems();
    } // namespace GraphicSystems
} // namespace Systems
