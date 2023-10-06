/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include "CustomTypes.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

namespace Systems {

    void windowCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player), typeid(Types::Position), typeid(Types::CollisionRect)});

        const float maxPercent = 100.0F;
        for (std::size_t id : ids) {
            if (arrPosition[id].x < 0) {
                arrPosition[id].x = 0;
            }
            if (arrPosition[id].y < 0) {
                arrPosition[id].y = 0;
            }
            if (arrPosition[id].x + arrCollisionRect[id].width > maxPercent) {
                arrPosition[id].x = maxPercent - arrCollisionRect[id].width;
            }
            if (arrPosition[id].y + arrCollisionRect[id].height > maxPercent) {
                arrPosition[id].y = maxPercent - arrCollisionRect[id].height;
            }
        }
    }

    static bool checkAllies(std::size_t fstId, std::size_t scdId)
    {
        Registry &registry                          = Registry::getInstance();
        Registry::components<Types::Player> players = registry.getComponents<Types::Player>();
        Registry::components<Types::Enemy> enemys   = registry.getComponents<Types::Enemy>();
        Registry::components<Types::PlayerAllies> playerAllies =
            registry.getComponents<Types::PlayerAllies>();
        Registry::components<Types::EnemyAllies> enemyAllies = registry.getComponents<Types::EnemyAllies>();

        if ((playerAllies.exist(fstId) && players.exist(scdId))
            || (playerAllies.exist(scdId) && players.exist(fstId))) {
            return true;
        }
        if ((enemyAllies.exist(fstId) && enemys.exist(scdId))
            || (enemyAllies.exist(scdId) && enemys.exist(fstId))) {
            return true;
        }
        return false;
    }

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        Registry::components<Types::Damage> arrDamage =
            Registry::getInstance().getComponents<Types::Damage>();
        Registry::components<health_s> arrHealth = Registry::getInstance().getComponents<health_s>();

        if (checkAllies(firstEntity, secondEntity)) {
            return;
        }

        if (arrDamage.exist(firstEntity) && arrDamage[firstEntity].damage > 0) {
            if (arrHealth.exist(secondEntity)) {
                arrHealth[secondEntity].hp -= arrDamage[firstEntity].damage;
            }
        }
    }

    static void checkCollisionEntity(
        std::vector<size_t>::iterator itIds,
        std::vector<std::size_t> &ids,
        Registry::components<Types::Position> arrPosition,
        Registry::components<Types::CollisionRect> arrCollisionRect)
    {
        std::size_t id                  = *itIds;
        Types::Position entityPos       = arrPosition[id];
        Types::CollisionRect entityColl = arrCollisionRect[id];

        itIds++;
        while (itIds != ids.end()) {
            if (arrCollisionRect.exist(*itIds)) {
                Types::CollisionRect sndEntityRect = arrCollisionRect[*itIds];
                Types::Position sndEntityPos       = arrPosition[*itIds];
                if (entityPos.x < sndEntityPos.x + sndEntityRect.width
                    && entityPos.x + entityColl.width > sndEntityPos.x
                    && entityPos.y < sndEntityPos.y + sndEntityRect.height
                    && entityPos.y + entityColl.height > sndEntityPos.y) {
                    giveDamages(id, *itIds);
                    giveDamages(*itIds, id);
                }
            }
            itIds++;
        }
    }

    void entitiesCollision(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry                                = Registry::getInstance();
        Registry::components<Types::Position> arrPosition = registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(Types::CollisionRect), typeid(Types::Position)});

        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
        }
    }

#ifndef NDEBUG
    void debugCollisionRect(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::RectangleShape> arrRectangleShape =
            Registry::getInstance().getComponents<Types::RectangleShape>();

        std::vector<std::size_t> ids = arrCollisionRect.getExistingsId();

        for (auto &id : ids) {
            if (arrPosition.exist(id) && !arrRectangleShape.exist(id)) {
                Registry::getInstance().getComponents<Types::RectangleShape>().insert(
                    id,
                    {arrCollisionRect[id].width, arrCollisionRect[id].height});
            }
        }
    }
#endif

    void moveEntities(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::Velocity> arrVelocity =
            Registry::getInstance().getComponents<Types::Velocity>();

        std::vector<std::size_t> ids = arrPosition.getExistingsId();

        for (auto &id : ids) {
            if (arrVelocity.exist(id)) {
                arrPosition[id].x += arrVelocity[id].speedX;
                arrPosition[id].y += arrVelocity[id].speedY;
            }
        }
    }

    void checkDestroyAfterDeathCallBack(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry &registry = Registry::getInstance();
        auto deadList      = registry.getComponents<Types::Dead>();
        auto deadIdList    = deadList.getExistingsId();
        Clock &clock       = registry.getClock();

        for (auto id : deadIdList) {
            Types::Dead &dead = deadList[id];
            if (static_cast<int>(dead.clockId) > -1
                && clock.elapsedMillisecondsSince(dead.clockId) > dead.timeToWait) {
                clock.restart(dead.clockId);
                registry.removeEntity(id);
            }
        }
    }

    static void executeDeathFunction(std::size_t id, Registry::components<Types::Dead> arrDead)
    {
        Types::Dead &deadComp = arrDead[id];
        if (deadComp.deathFunction != std::nullopt) {
            if (!deadComp.launched) {
                deadComp.deathFunction.value()(id);
                deadComp.clockId  = Registry::getInstance().getClock().create();
                deadComp.launched = true;
            }
        } else {
            Registry::getInstance().removeEntity(id);
        }
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<health_s> arrHealth  = Registry::getInstance().getComponents<health_s>();
        Registry::components<Types::Dead> arrDead = Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            if (arrHealth.exist(*itIds) && arrHealth[*itIds].hp <= 0 && arrDead.exist(*itIds)) {
                executeDeathFunction(*itIds, arrDead);
            }
        }
    }

    const std::string musicPath              = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath              = "assets/Audio/Sounds/fire.ogg";
    const std::string playerPath             = "assets/R-TypeSheet/r-typesheet14.gif";
    const std::string ennemyPath             = "assets/R-TypeSheet/r-typesheet18.gif";
    const Types::Rect spriteRect             = {2, 2, 48, 48};
    const Types::CollisionRect collisionRect = {25, 25};
    const Raylib::Vector2 textPos            = {5, 5};
    constexpr int playerData                 = 10;
    const Types::Position playerPos          = {50, 50};
    constexpr int playerDamage               = 1;
    constexpr int enemyDamage                = 1;
    constexpr int playerHealth               = 5;
    constexpr int playerHealth2              = 5;
    constexpr float musicVolume              = 0.02F;
    constexpr float soundVolume              = 0.1F;
    constexpr float fontScale                = 2.0F;
    const float playerWidth                  = 25.0F;
    const float playerHeight                 = 25.0F;
    const std::size_t deadTime               = 1500;

    void init(std::size_t managerId, std::size_t systemId)
    {
        std::size_t id = Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {playerData, playerData + playerData + playerData});
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {playerPath, playerWidth, playerHeight, id});
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack({
            spriteRect,
            {spriteRect, {51, 1, 46, 47}, {101, 2, 48, 47},     {152, 2, 46, 47},          {201, 2, 46, 47}},
            {{2, 51, 46, 47},          {101, 2, 48, 47},           {152, 2, 46, 47},      {201, 2, 46, 47}    },
            {{180, 140, 18, 12},          {211, 140, 18, 12},        {230, 140, 18, 12}, {250, 140, 18, 12}}
        });
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        Registry::getInstance().setToBackLayers(id);
        Registry::getInstance().getComponents<Types::Player>().insertBack({});
        Registry::getInstance().getComponents<Types::Damage>().insertBack({playerDamage});
        Registry::getInstance().getComponents<Types::Health>().insertBack({playerHealth});
        Registry::getInstance().getComponents<Types::Dead>().insertBack({std::nullopt, deadTime});

        id = Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Enemy>().insertBack({});
        Registry::getInstance().getComponents<Types::Position>().insertBack(playerPos);
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {playerPath, playerWidth, playerHeight, id});
        Registry::getInstance().getComponents<Types::Rect>().insertBack(spriteRect);
        Registry::getInstance().getComponents<Types::CollisionRect>().insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Dead>().insertBack({std::nullopt});
        Registry::getInstance().setToFrontLayers(id);

        Registry::getInstance().getComponents<Types::Damage>().insertBack({enemyDamage});
        Registry::getInstance().getComponents<Types::Health>().insertBack({playerHealth2});

        id = Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Raylib::Music>().insertBack({musicPath, musicVolume});
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack({soundPath, soundVolume});
        Registry::getInstance().getComponents<Raylib::Text>().insertBack(
            {"Press SPACE to play music, ENTER to play sound, J to reset "
             "scene, ARROWS to move ",
             textPos,
             fontScale,
             Raylib::DarkBlue});
        SystemManagersDirector::getInstance().getSystemManager(managerId).removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {
            windowCollision,
            init,
            entitiesCollision,
            moveEntities,
            checkDestroyAfterDeathCallBack,
#ifndef NDEBUG
            debugCollisionRect,
#else
#endif
            deathChecker};
    }
} // namespace Systems
