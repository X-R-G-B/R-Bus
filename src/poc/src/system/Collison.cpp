/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Collison
*/

#include <iostream>
#include "Collison.hpp"

namespace System {
    
    Collison::Collison(Registry *registry)
        : ASystem(registry)
    {

    }

    void Collison::run()
    {
        std::cout << "Increment sparse array int of 1" << std::endl;
        Registry::array<int> arrInt = _registry->getComponents<int>();

        for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
            *begin += 1;
        }
        std::cout << "------------------------------------" << std::endl;
    }
}
