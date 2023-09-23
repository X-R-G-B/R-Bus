/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include <iostream>
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {
    void collision(std::size_t)
    {
        Registry::components<Pixel> arrPixel =
        Registry::getInstance().getComponents<Pixel>();

        for (auto begin = arrPixel.begin(); begin != arrPixel.end(); begin++) {
            if (!begin->has_value()) {
                continue;
            }
            if (begin->value().x < 0)
                begin->value().x = 0;
            if (begin->value().x > 750)
                begin->value().x = 750;
            if (begin->value().y < 0)
                begin->value().y = 0;
            if (begin->value().y > 400)
                begin->value().y = 400;
        }
    }
} // namespace Systems
