/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <array>
#include "Systems.hpp"
#include "GraphicSystems.hpp"
#include "EventsSystems.hpp"

namespace Systems {
    const std::array<std::vector<std::function<void(std::size_t)>>, 3> systemsGroups {
        ecsSystems,
        EventsSystems::eventSystems,
        GraphicSystems::graphicSystems
    };
} // namespace Systems
