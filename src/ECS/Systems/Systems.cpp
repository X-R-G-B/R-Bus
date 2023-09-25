/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include <cstddef>
#include <iostream>
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {
    void collision(std::size_t /*unused*/)
    {
        std::cout << "aa" << std::endl;
    }
    constexpr std::list<std::function<void(std::size_t)>> ecsSystems {
        collision
    }
} // namespace Systems
