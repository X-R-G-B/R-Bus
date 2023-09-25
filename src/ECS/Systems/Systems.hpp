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
    void collision(std::size_t);
    extern constexpr std::list<std::function<void(std::size_t)>> ecsSystems;
}
