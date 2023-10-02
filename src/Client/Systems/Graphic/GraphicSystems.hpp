/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicSystems {
        std::vector<std::function<void(std::size_t, std::size_t)>>
        getGraphicsSystems();
    } // namespace GraphicSystems
} // namespace Systems
