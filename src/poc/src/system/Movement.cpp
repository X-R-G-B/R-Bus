/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Movement
*/

#include <iostream>
#include "Movement.hpp"

namespace System {
    Movement::Movement(Registry *registry)
        : ASystem(registry)
    {
        
    }

    void Movement::run()
    {
        std::cout << "Printing sparse array of int" << std::endl;
        Registry::array<int> arrInt = _registry->getComponents<int>();

        for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
            std::cout << *begin << std::endl;
        }
        std::cout << "------------------------------------" << std::endl;
    }
}
