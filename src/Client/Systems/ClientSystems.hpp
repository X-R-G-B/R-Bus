/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#pragma once

#include <array>
#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    std::array<std::vector<std::function<void(std::size_t, std::size_t)>>, 8> getSystemsGroups();
} // namespace Systems
