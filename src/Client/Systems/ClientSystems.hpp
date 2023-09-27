/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#pragma once

#include <array>
#include "Systems.hpp"
#include "GraphicSystems.hpp"
#include "EventsSystems.hpp"

namespace Systems {
    const std::array<std::vector<std::function<void(std::size_t)>>, 3> getSystemsGroups();
} // namespace Systems
