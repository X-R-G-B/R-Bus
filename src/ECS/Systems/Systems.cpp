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
        Registry::components<Types::Position> arrPosition =
        Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
        Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Player> arrPlayer =
        Registry::getInstance().getComponents<Types::Player>();

        auto positionIt  = arrPosition.begin();
        auto collisionIt = arrCollisionRect.begin();
        auto playerIt    = arrPlayer.begin();

        while (playerIt != arrPlayer.end() && positionIt != arrPosition.end()
               && collisionIt != arrCollisionRect.end()) {
            if (
            playerIt->has_value() && positionIt->has_value()
            && collisionIt->has_value()) {
                if (positionIt->value().x < 0) {
                    positionIt->value().x = 0;
                }
                if (positionIt->value().y < 0) {
                    positionIt->value().y = 0;
                }
                if (positionIt->value().x + collisionIt->value().width > 100) {
                    positionIt->value().x = 100 - collisionIt->value().width;
                }
                if (positionIt->value().y + collisionIt->value().height > 100) {
                    positionIt->value().y = 100 - collisionIt->value().height;
                }
            }
            positionIt++;
            collisionIt++;
            playerIt++;
        }
    }
} // namespace Systems
