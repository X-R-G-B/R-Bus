/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {
    void EventsSystems::playerMovement(std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();

        auto positionIt = arrPosition.begin();
        auto playerIt   = arrPlayer.begin();

        while (positionIt != arrPosition.end() && playerIt != arrPlayer.end()) {
            if (playerIt->has_value() && positionIt->has_value()
                && playerIt->value().isMine) {
                if (IsKeyDown(KEY_RIGHT)) {
                    positionIt->value().x += 1;
                }
                if (IsKeyDown(KEY_LEFT)) {
                    positionIt->value().x -= 1;
                }
                if (IsKeyDown(KEY_UP)) {
                    positionIt->value().y -= 1;
                }
                if (IsKeyDown(KEY_DOWN)) {
                    positionIt->value().y += 1;
                }
            }
            positionIt++;
            playerIt++;
        }
    }
    std::vector<std::function<void(std::size_t)>>
    EventsSystems::getEventSystems()
    {
        return {playerMovement};
    }
} // namespace Systems
