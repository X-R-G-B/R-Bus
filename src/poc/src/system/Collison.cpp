/*
** Collison.cpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 9:36:31 AM 2023 brice
** Last update Thu Sep 20 12:03:08 PM 2023 brice
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
