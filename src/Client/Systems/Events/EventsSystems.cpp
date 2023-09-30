/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"

namespace Systems {
    void EventsSystems::playerMovement(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();

        std::vector<std::size_t> playerId = Registry::getInstance().getCustomSparseArray<Types::Player>().getExistingsId();

        for (std::size_t id : playerId) {
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                arrPosition[id].x += 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                arrPosition[id].x -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                arrPosition[id].y -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                arrPosition[id].y += 1;
            }
        }
    }

    void
    EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_J)) {
            SceneManager &sceneManager = SceneManager::getInstance();
            if (sceneManager.getCurrentScene() == MAIN_GAME) {
                sceneManager.changeScene(MENU);
            } else {
                sceneManager.changeScene(MAIN_GAME);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene};
    }
} // namespace Systems
