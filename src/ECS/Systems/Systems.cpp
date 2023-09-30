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

    const std::string musicPath  = "assets/Audio/Musics/Title.mp3";
    const std::string soundPath  = "assets/Audio/Sounds/fire.ogg";
    const std::string playerPath = "assets/R-TypeSheet/r-typesheet14.gif";
    const Types::Rect spriteRect = {2, 2, 48, 48};
    const Types::CollisionRect collisionRect = {46, 46};
    const Raylib::Vector2 textPos            = {20, 50};
    constexpr int playerData                 = 10;
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
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {windowCollision, init};
    }
} // namespace Systems
