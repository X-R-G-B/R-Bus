/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "Systems.hpp"

namespace Systems {
    static void checkAnimRect(std::size_t id, Clock &clock_, std::size_t clockId)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        clock_.restart(clockId);

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            if (anim.currentRectList != Types::RectListType::MOVE) {
                anim.changeRectList(Types::RectListType::MOVE);
            }
        }
    }

    constexpr std::size_t elapsedBetweenMove = 20;

    void EventsSystems::playerMovement(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                              = Registry::getInstance();
        Registry::components<Types::Position> arrPos    = registry.getComponents<Types::Position>();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids                    = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(struct health_s)});
        Clock &clock_              = registry.getClock();
        static std::size_t clockId = clock_.create(true);

        for (auto id : ids) {
            if (clock_.elapsedMillisecondsSince(clockId) < elapsedBetweenMove || arrHealth[id].hp <= 0) {
                continue;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].x += 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].x -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].y -= 1;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].y += 1;
            }
        }
    }

    const std::size_t waitTimeBullet = 500;

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Clock &clock_                                     = registry.getClock();
        static std::size_t clockId                        = clock_.create(true);
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::Player), typeid(Types::Position)});

        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_SPACE)
            && clock_.elapsedMillisecondsSince(clockId) > waitTimeBullet) {
            clock_.restart(clockId);
            for (auto &id : ids) {
                // send bullet to server
                Nitwork::NitworkClient::getInstance().addNewBulletMsg(
                    {static_cast<int>(arrPosition[id].x), static_cast<int>(arrPosition[id].y)},
                    CLASSIC);
                struct Types::Missiles missile = {
                    .type = CLASSIC,
                };
                createMissile(arrPosition[id], missile);
            }
        }
    }

    void EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_J)) {
            auto &sceneManager = Scene::SceneManager::getInstance();
            if (sceneManager.getCurrentScene() == Scene::Scene::MAIN_GAME) {
                sceneManager.changeScene(Scene::Scene::MENU);
            } else {
                sceneManager.changeScene(Scene::Scene::MAIN_GAME);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene, playerShootBullet};
    }
} // namespace Systems
