/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Movement
*/

#include <cstdlib>
#include <iostream>
#include "Movement.hpp"
#include "CustomTypes.hpp"

namespace System {
    Movement::Movement(Registry *registry)
        : ASystem(registry)
    {
        
    }

    void Movement::run()
    {
        std::cout << "Printing sparse array of int" << std::endl;
        Registry::array<int> arrInt = _registry->getComponents<int>();
        Registry::array<Pixel> arrPixel = _registry->getComponents<Pixel>();

        for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
            std::cout << *begin << std::endl;
        }
        for (auto begin = arrPixel->begin(); begin != arrPixel->end(); begin++) {
            begin->x = rand() % 750;
            begin->y = rand() % 400;
        }
        std::cout << "------------------------------------" << std::endl;
    }
}
