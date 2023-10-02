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
    void windowCollision(std::size_t, std::size_t);
    void init(std::size_t, std::size_t);
    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems();
} // namespace Systems
