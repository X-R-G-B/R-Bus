/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {
    void windowCollision(std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();
        const float maxPercent = 100.0F;

        auto positionIt  = arrPosition.begin();
        auto collisionIt = arrCollisionRect.begin();
        auto playerIt    = arrPlayer.begin();

        while (playerIt != arrPlayer.end() && positionIt != arrPosition.end()
               && collisionIt != arrCollisionRect.end()) {
            if (playerIt->has_value() && positionIt->has_value()
                && collisionIt->has_value()) {
                if (positionIt->value().x < 0) {
                    positionIt->value().x = 0;
                }
                if (positionIt->value().y < 0) {
                    positionIt->value().y = 0;
                }
                if (positionIt->value().x + collisionIt->value().width
                    > maxPercent) {
                    positionIt->value().x =
                        maxPercent - collisionIt->value().width;
                }
                if (positionIt->value().y + collisionIt->value().height
                    > maxPercent) {
                    positionIt->value().y =
                        maxPercent - collisionIt->value().height;
                }
            }
            positionIt++;
            collisionIt++;
            playerIt++;
        }
    }

    std::vector<std::function<void(std::size_t)>> getECSSystems()
    {
        return {windowCollision};
    }
} // namespace Systems
