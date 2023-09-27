/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "Registry.hpp"
#include "CustomTypes.hpp"

#include "SceneManager.hpp"

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
                    std::cout << "UP" << std::endl;
                    SceneManager::getInstance().changeScene(Scene::MENU);
                    positionIt->value().y -= 1;
                }
                if (IsKeyDown(KEY_DOWN)) {
                    std::cout << "DOWN" << std::endl;
                    SceneManager::getInstance().changeScene(Scene::MAIN_GAME);
                    positionIt->value().y += 1;
                }
            }
            positionIt++;
            playerIt++;
        }
    }
    const std::vector<std::function<void(std::size_t)>> EventsSystems::getEventSystems()
    {
        return {
            playerMovement
        };
    }
}
