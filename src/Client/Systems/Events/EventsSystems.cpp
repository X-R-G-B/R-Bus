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
    static void checkAnimRect(std::size_t id)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            if (anim.currentRectList != Types::RectListType::MOVE) {
                anim.changeRectList(Types::RectListType::MOVE);
            }
        }
    }

    void EventsSystems::playerMovement(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPos =
            registry.getComponents<Types::Position>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position)});

        for (auto id : ids) {
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id);
                arrPos[id].x += 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id);
                arrPos[id].x -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id);
                arrPos[id].y -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id);
                arrPos[id].y += 1;
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
