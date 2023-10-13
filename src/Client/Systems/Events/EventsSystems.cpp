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
#include "Maths.hpp"

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
        static constexpr int speed = 1;

        for (auto id : ids) {
            if (clock_.elapsedMillisecondsSince(clockId) < elapsedBetweenMove || arrHealth[id].hp <= 0) {
                continue;
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_RIGHT)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].x += Maths::addIntegerDecimals(speed);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_LEFT)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].x -= Maths::addIntegerDecimals(speed);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_UP)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].y -= Maths::addIntegerDecimals(speed);
            }
            if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_DOWN)) {
                checkAnimRect(id, clock_, clockId);
                arrPos[id].y += Maths::addIntegerDecimals(speed);
            }
        }
    }

    static void createMissile(Types::Position &pos)
    {
        std::size_t entityId = Registry::getInstance().addEntity();

        Types::Rect spriteRect               = {200, 121, 32, 10};
        const std::string bulletPath         = "assets/R-TypeSheet/r-typesheet1.gif";
        constexpr int bulletWidth          = 5;
        constexpr int bulletHeight         = 5;
        Types::CollisionRect collisionRect1  = {1, 1};
        Types::Velocity velocity             = {70, 0};
        Types::Missiles missileType          = {CLASSIC};
        Types::Dead deadComp                 = {};
        Types::PlayerAllies playerAlliesComp = {};
        Types::Position position             = {pos.x, pos.y};
        Types::CollisionRect collisionRect2  = {bulletWidth, bulletHeight};
        Raylib::Sprite sprite                = {bulletPath, bulletWidth, bulletHeight, entityId};
        struct health_s healthComp           = {1};
        Types::Damage damageComp             = {10};

        Registry::getInstance().getComponents<Types::Position>().insertBack(position);
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(sprite);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect1);
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect2);
        Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
        Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack(playerAlliesComp);
        Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
        Registry::getInstance().getComponents<struct health_s>().insertBack(healthComp);
        Registry::getInstance().getComponents<Types::Damage>().insertBack(damageComp);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(deadComp);
        Registry::getInstance().setToFrontLayers(entityId);
        // send bullet to server
        Nitwork::NitworkClient::getInstance().addNewBulletMsg(
            {
                Maths::removeIntegerDecimals(position.x),
                Maths::removeIntegerDecimals(position.y)
            },
            missileType.type);
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

        if (Raylib::isKeyPressed(Raylib::KeyboardKey::KB_SPACE)
            && clock_.elapsedMillisecondsSince(clockId) > waitTimeBullet) {
            clock_.restart(clockId);
            for (auto &id : ids) {
                createMissile(arrPosition[id]);
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
