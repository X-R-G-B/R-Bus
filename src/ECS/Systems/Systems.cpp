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
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();

        std::vector<std::size_t> playerIdx = Registry::getInstance()
                                                 .getComponents<Types::Player>()
                                                 .getExistingsId();

        const float maxPercent = 100.0F;
        for (std::size_t id : playerIdx) {
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
        Registry::components<Types::Dammage> arrDammage =
            Registry::getInstance().getComponents<Types::Dammage>();
        Registry::components<Types::Health> arrHealth =
            Registry::getInstance().getComponents<Types::Health>();

        if (arrDammage.exist(firstEntity)
            && arrDammage[firstEntity].dammage > 0) {
            if (arrHealth.exist(secondEntity)
                && arrHealth[secondEntity].hp > 0) {
                arrHealth[secondEntity].hp -= arrDammage[firstEntity].dammage;
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
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();

        std::vector<std::size_t> ids = arrPosition.getExistingsId();
        auto itIds                   = ids.begin();

        while (itIds != ids.end()) {
            if (arrCollisionRect.exist(*itIds)) {
                checkCollisionEntity(itIds, ids, arrPosition, arrCollisionRect);
            }
            itIds++;
        }
    }

    const std::string musicPath  = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath  = "assets/Audio/Sounds/fire.ogg";
    const std::string playerPath = "assets/R-TypeSheet/r-typesheet14.gif";
    const Types::Rect spriteRect = {2, 2, 48, 48};
    const Types::CollisionRect collisionRect = {46, 46};
    const Raylib::Vector2 textPos            = {20, 50};
    constexpr int playerData                 = 10;
    constexpr int playerDammage              = 10;
    constexpr int playerHealth               = 1;
    constexpr float musicVolume              = 0.02F;
    constexpr float soundVolume              = 0.1F;
    constexpr float fontScale                = 2.0F;
    const float playerWidth                  = 50.0F;
    const float playerHeight                 = 50.0F;

    void init(std::size_t managerId, std::size_t systemId)
    {
        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Position>().insertBack(
            {playerData, playerData});
        Registry::getInstance().getComponents<Raylib::Sprite>().insertBack(
            {playerPath, playerWidth, playerHeight});
        Registry::getInstance().getComponents<Types::Rect>().insertBack(
            spriteRect);
        Registry::getInstance()
            .getComponents<Types::CollisionRect>()
            .insertBack(collisionRect);
        Registry::getInstance().getComponents<Types::Player>().insertBack({});
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
        Registry::getInstance().getComponents<Types::Dammage>().insertBack(
            {playerDammage});
        Registry::getInstance().getComponents<Types::Health>().insertBack(
            {playerHealth});
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {windowCollision, init, entitiesCollision};
    }
} // namespace Systems
