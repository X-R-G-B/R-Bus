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
    }

    void movement(std::size_t)
    {
        static auto registry = Registry::getInstance();
        std::cout << "Printing sparse array of int" << std::endl;
    }
}
