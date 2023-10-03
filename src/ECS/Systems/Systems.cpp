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
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::Player),
             typeid(Types::Position),
             typeid(Types::CollisionRect)});

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

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        Registry::components<Types::Damage> arrDamage =
            Registry::getInstance().getComponents<Types::Damage>();
        Registry::components<Types::Health> arrHealth =
            Registry::getInstance().getComponents<Types::Health>();

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
        Registry &registry = Registry::getInstance();
        Registry::components<Types::Position> arrPosition =
            registry.getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            registry.getComponents<Types::CollisionRect>();
        std::vector<std::size_t> ids = registry.getEntitiesByComponents(
            {typeid(Types::CollisionRect), typeid(Types::Position)});

        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
        }
    }

    static void executeDeathFunction(
        std::size_t id,
        Registry::components<Types::Dead> arrDead)
    {
        if (arrDead[id].deathFunction != std::nullopt) {
            arrDead[id].deathFunction.value()(id);
        } else {
            Registry::getInstance().removeEntity(id);
        }
    }

    void deathChecker(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        Registry::components<Types::Health> arrHealth =
            Registry::getInstance().getComponents<Types::Health>();
        Registry::components<Types::Dead> arrDead =
            Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrHealth.getExistingsId();
        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            if (arrHealth.exist(*itIds) && arrHealth[*itIds].hp <= 0
                && arrDead.exist(*itIds)) {
                executeDeathFunction(*itIds, arrDead);
            }
        }
    }

    const std::string musicPath  = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath  = "assets/Audio/Sounds/fire.ogg";
    const std::string playerPath = "assets/R-TypeSheet/r-typesheet14.gif";
    const Types::Rect spriteRect = {2, 2, 48, 48};
    const Types::CollisionRect collisionRect = {25, 25};
    const Raylib::Vector2 textPos            = {20, 50};
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

    void init(std::size_t managerId, std::size_t systemId)
    {
        std::size_t id = Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            playerPos);
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {playerPath, playerWidth, playerHeight, id});
        Registry::getInstance().getComponents<Types::Rect>().insertBack(
            spriteRect);
        Registry::getInstance()
            .getComponents<Types::CollisionRect>()
            .insertBack(collisionRect);
        // NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        Registry::getInstance().getComponents<Types::AnimRect>().insertBack({
            spriteRect,
            {{2, 51, 46, 47},
              {101, 2, 48, 47},
              {152, 2, 46, 47},
              {201, 2, 46, 47}},
            {{2, 51, 46, 47},
              {101, 2, 48, 47},
              {152, 2, 46, 47},
              {201, 2, 46, 47}},
            {{180, 140, 18, 12},
              {211, 140, 18, 12},
              {230, 140, 18, 12},
              {250, 140, 18, 12}}
        });
        // NOLINTEND(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        Registry::getInstance().setToBackLayers(id);
        Registry::getInstance().getComponents<Types::Player>().insertBack({});
        Registry::getInstance().getComponents<Types::Damage>().insertBack(
            {playerDamage});
        Registry::getInstance().getComponents<Types::Health>().insertBack(
            {playerHealth});
        Registry::getInstance().getComponents<Types::Dead>().insertBack(
            {std::nullopt});

        id = Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Enemy>().insertBack({});
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {playerData, playerData + playerData + playerData});
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {playerPath, playerWidth, playerHeight, id});
        Registry::getInstance().getComponents<Types::Rect>().insertBack(
            spriteRect);
        Registry::getInstance()
            .getComponents<Types::CollisionRect>()
            .insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Dead>().insertBack(
            {std::nullopt});
        Registry::getInstance().setToFrontLayers(id);

        Registry::getInstance().getComponents<Raylib::Music>().insertBack(
            {musicPath, musicVolume});
        Registry::getInstance().getComponents<Raylib::Sound>().insertBack(
            {soundPath, soundVolume});
        Registry::getInstance().getComponents<Raylib::Text>().insertBack(
            {"Press SPACE to play music, ENTER to play sound, J to reset "
             "scene, ARROWS to move",
             textPos,
             fontScale,
             Raylib::DarkBlue});
        Registry::getInstance().getComponents<Types::Damage>().insertBack(
            {enemyDamage});
        Registry::getInstance().getComponents<Types::Health>().insertBack(
            {playerHealth2});
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {windowCollision, init, entitiesCollision, deathChecker};
    }
} // namespace Systems
