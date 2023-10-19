/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems implementation
*/

#include "EventsSystems.hpp"
#include "CustomTypes.hpp"
#include "Maths.hpp"
#include "NitworkClient.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SceneManager.hpp"
#include "Systems.hpp"

namespace Systems {
    static void checkAnimRect(std::size_t id, Clock &clock_, std::size_t clockId , Types::Direction direction)
    {
        Registry::components<Types::AnimRect> arrAnimRect =
            Registry::getInstance().getComponents<Types::AnimRect>();

        clock_.restart(clockId);

        if (arrAnimRect.exist(id)) {
            Types::AnimRect &anim = arrAnimRect[id];
            anim.changeRectList(Types::RectListType::MOVE);
            anim.changeDirection(direction);
        }
    }

    constexpr std::size_t elapsedBetweenMove = 20;

    void EventsSystems::playerMovement(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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
                checkAnimRect(id, clock_, clockId, Types::Direction::RIGHT);
                Maths::addNormalIntToDecimalInt(arrPos[id].x, 1);
                return;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::LEFT);
                Maths::subNormalIntToDecimalInt(arrPos[id].x, 1);
                return;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::UP);
                Maths::subNormalIntToDecimalInt(arrPos[id].y, 1);
                return;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId, Types::Direction::DOWN);
                Maths::addNormalIntToDecimalInt(arrPos[id].y, 1);
                return;
            }
            checkAnimRect(id, clock_, clockId, Types::Direction::NONE);
        }
    }

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        static const std::size_t waitTimeBullet           = 500;
        static const std::string soundPathShoot           = "assets/Audio/Sounds/laser.ogg";
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<struct health_s> arrHealth   = registry.getComponents<struct health_s>();
        Clock &clock_                                     = registry.getClock();
        static std::size_t clockId                        = clock_.create(true);
        Registry::components<Raylib::Sound> arrSounds     = registry.getComponents<Raylib::Sound>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::Player), typeid(Types::Position)});

        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_SPACE) == false
            || clock_.elapsedMillisecondsSince(clockId) < waitTimeBullet) {
            return;
        }

        for (auto &sound : arrSounds) {
            if (sound.getPath() == soundPathShoot) {
                sound.setNeedToPlay(true);
                break;
            }
        }

        for (auto &id : ids) {
            // send bullet to server
            if (arrHealth.exist(id) && arrHealth[id].hp <= 0) {
                continue;
            }
            Nitwork::NitworkClient::getInstance().addNewBulletMsg(
                {Maths::removeIntDecimals(arrPosition[id].x), Maths::removeIntDecimals(arrPosition[id].y)},
                CLASSIC);
            struct Types::Missiles missile = {
                .type = CLASSIC,
            };
            createMissile(arrPosition[id], missile);
            clock_.restart(clockId);
        }
    }

    void EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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
