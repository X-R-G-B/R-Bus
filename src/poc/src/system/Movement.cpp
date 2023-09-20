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
#include "EventManager.hpp"
#include "raylib.h"

namespace System {
    Movement::Movement(Registry *registry)
        : ASystem(registry)
    {
    }

    void Movement::run()
    {
        EventManager &eventManager = EventManager::getInstance();
        Registry::array<int> arrInt = _registry->getComponents<int>();
        Registry::array<Pixel> arrPixel = _registry->getComponents<Pixel>();

        for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
            std::cout << *begin << std::endl;
        }
        for (auto begin = arrPixel->begin(); begin != arrPixel->end(); begin++) {
            if (eventManager.checkEvent(KEY_LEFT))
                begin->x -= 1;
            if (eventManager.checkEvent(KEY_RIGHT))
                begin->x += 1;
            if (eventManager.checkEvent(KEY_UP))
                begin->y -= 1;
            if (eventManager.checkEvent(KEY_DOWN))
                begin->y += 1;
        }
        std::cout << "------------------------------------" << std::endl;
    }
}
