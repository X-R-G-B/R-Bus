/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** TextSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicsSystems {
        void textRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        std::vector<std::function<void(std::size_t, std::size_t)>> getTextSystems();
    } // namespace GraphicsSystems
} // namespace Systems
