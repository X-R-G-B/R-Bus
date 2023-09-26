/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems
*/

#pragma once

#include <cstddef>
#include <list>

namespace Systems {
    void windowCollision(std::size_t);
    extern const std::list<std::function<void(std::size_t)>> ecsSystems;
} // namespace Systems
