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
        const int xMax = 750;
        const int yMax = 400;
        Registry::components<Pixel> arrPixel =
        Registry::getInstance().getComponents<Pixel>();

        for (auto &begin : arrPixel) {
            if (!begin.has_value()) {
                continue;
            }
            if (begin.value().x < 0) {
                begin.value().x = 0;
            }
            if (begin.value().x > xMax) {
                begin.value().x = xMax;
            }
            if (begin.value().y < 0) {
                begin.value().y = 0;
            }
            if (begin.value().y > yMax) {
                begin.value().y =yMax;
            }
        }
    }
} // namespace Systems
