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
    void windowCollision(std::size_t)
    {
        Types::Position *position = nullptr;

        Registry::components<Types::Position> arrPosition =
        Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
        Registry::getInstance().getComponents<Types::CollisionRect>();

        for (auto &pos : arrPosition) {
            position = &pos;
        }

        for (auto begin = arrCollisionRect.begin();
             begin != arrCollisionRect.end() && position != nullptr; begin++) {
            if (position->x < 0) {
                position->x = 0;
            }
            if (position->y < 0) {
                position->y = 0;
            }
            if (position->x + begin->width > 100) {
                position->x = 100 - begin->width;
            }
            if (position->y + begin->height > 100) {
                position->y = 100 - begin->height;
            }
        }
    }
} // namespace Systems
