/*
** Movement.cpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 10:19:53 AM 2023 brice
** Last update Thu Sep 20 2:34:00 PM 2023 brice
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
