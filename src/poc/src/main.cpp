/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include <unistd.h>
#include "SystemManager.hpp"
#include "Collison.hpp"
#include "Movement.hpp"

int main()
{
    Registry::getInstance();
    Registry::array<int> arrInt = Registry::getInstance().registerComponent<int>();
    arrInt->add(4);
    arrInt->add(69);
    Registry::array<float> arrFloat = Registry::getInstance().registerComponent<float>();
    arrFloat->add(69.69);
    Registry::array<float> scdContainer = Registry::getInstance().getComponents<float>();
    for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
        std::cout << *begin << std::endl;
    }
    for (auto begin = scdContainer->begin(); begin != scdContainer->end(); begin++) {
        std::cout << *begin << std::endl;
    }
    return 0;
}
