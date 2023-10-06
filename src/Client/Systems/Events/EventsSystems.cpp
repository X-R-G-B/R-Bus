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
        Registry &registry                            = Registry::getInstance();
        Registry::components<Types::Position> arrPos  = registry.getComponents<Types::Position>();
        Registry::components<Types::Health> arrHealth = registry.getComponents<Types::Health>();
        std::vector<std::size_t> ids                  = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(Types::Health)});
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

    const Types::Rect spriteRect             = {300, 121, 32, 10};
    const std::string bulletPath             = "assets/R-TypeSheet/r-typesheet1.gif";
    constexpr float bulletWidth              = 5.0F;
    constexpr float bulletHeight             = 5.0F;
    const Types::CollisionRect collisionRect = {1, 1};
    const Types::Velocity velocity           = {0.7F, 0.0F};
    const Types::Missiles missileType        = {Types::MissileTypes::CLASSIC};
    const health_s health                    = {1};
    const Types::Damage damage               = {10};

    static void createMissile(std::size_t id, Registry::components<Types::Position> &arrPosition)
    {
        if (arrPosition.exist(id)) {
            std::size_t entityId = Registry::getInstance().addEntity();

            Registry::getInstance().getComponents<Types::Position>().insertBack(
                {arrPosition[id].x, arrPosition[id].y});
            Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
                {bulletPath, bulletWidth, bulletHeight, entityId});
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(
                {bulletWidth, bulletHeight});
            Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
            Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
            Registry::getInstance().getComponents<Types::Missiles>().insertBack(missileType);
            Registry::getInstance().getComponents<Types::PlayerAllies>().insertBack({});
            Registry::getInstance().getComponents<Types::Velocity>().insertBack(velocity);
            Registry::getInstance().getComponents<health_s>().insertBack(health);
            Registry::getInstance().getComponents<Types::Damage>().insertBack(damage);
            Registry::getInstance().setToFrontLayers(entityId);
        }
    }

    const std::size_t waitTimeBullet = 1;

    void playerShootBullet(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Player> arrPlayer     = registry.getComponents<Types::Player>();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Clock &clock_                                     = registry.getClock();
        static std::size_t clockId                        = clock_.create(true);

        std::vector<std::size_t> ids = arrPlayer.getExistingsId();

        if (Raylib::isKeyDown(Raylib::KeyboardKey::KB_SPACE)
            && clock_.elapsedSecondsSince(clockId) > waitTimeBullet) {
            clock_.restart(clockId);
            for (auto &id : ids) {
                createMissile(id, arrPosition);
            }
        }
    }

    void EventsSystems::changeScene(std::size_t /*unused*/, std::size_t /*unused*/)
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

    std::vector<std::function<void(std::size_t, std::size_t)>> EventsSystems::getEventSystems()
    {
        return {playerMovement, changeScene, playerShootBullet};
    }
} // namespace Systems
