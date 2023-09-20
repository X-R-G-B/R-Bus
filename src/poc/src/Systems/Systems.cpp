/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include <iostream>
#include "Registry.hpp"

namespace Systems {
    void collision(std::size_t)
    {
        static Registry &registry = Registry::getInstance();
        std::cout << "Increment sparse array int of 1" << std::endl;
        //Registry::array<int> arrInt = registry.getComponents<int>();
//
        //for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
        //    *begin += 1;
        //}
        //std::cout << "------------------------------------" << std::endl;
    }

    void movement(std::size_t)
    {
        static auto registry = Registry::getInstance();
        std::cout << "Printing sparse array of int" << std::endl;
        Registry::array<int> arrInt = registry.getComponents<int>();
//
        //for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
        //    std::cout << *begin << std::endl;
        //}
        //std::cout << "------------------------------------" << std::endl;
    }
}
