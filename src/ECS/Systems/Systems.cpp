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
            if (begin->x < 0)
                begin->x = 0;
            if (begin->x > 750)
                begin->x = 750;
            if (begin->y < 0)
                begin->y = 0;
            if (begin->y > 400)
                begin->y = 400;
        }
    }
} // namespace Systems
