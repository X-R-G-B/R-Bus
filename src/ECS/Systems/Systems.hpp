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
    void windowCollision(std::size_t);
    const std::vector<std::function<void(std::size_t)>> ecsSystems {
        windowCollision
    };
} // namespace Systems
