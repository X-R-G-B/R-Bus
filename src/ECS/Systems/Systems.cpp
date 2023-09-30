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

        SparseArray<std::size_t> &playerId =
            Registry::getInstance().getCustomSparseArray<std::size_t>(
                CustomIndex::PLAYER);

        const float maxPercent = 100.0F;
        for (std::optional<std::size_t> id : playerId) {
            if (id.has_value() && arrPosition[id.value()].has_value()
                && arrCollisionRect[id.value()].has_value()) {
                if (arrPosition[id.value()].value().x < 0) {
                    arrPosition[id.value()].value().x = 0;
                }
                if (arrPosition[id.value()].value().y < 0) {
                    arrPosition[id.value()].value().y = 0;
                }
                if (arrPosition[id.value()].value().x
                        + arrCollisionRect[id.value()].value().width
                    > maxPercent) {
                    arrPosition[id.value()].value().x =
                        maxPercent - arrCollisionRect[id.value()].value().width;
                }
                if (arrPosition[id.value()].value().y
                        + arrCollisionRect[id.value()].value().height
                    > maxPercent) {
                    arrPosition[id.value()].value().y = maxPercent
                        - arrCollisionRect[id.value()].value().height;
                }
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
        Registry::getInstance().getComponents<Types::Position>().back() = {
            playerData,
            playerData};
        Registry::getInstance().getComponents<Raylib::Sprite>().back() = {
            playerPath,
            playerWidth,
            playerHeight};
        Registry::getInstance().getComponents<Types::Rect>().back() =
            spriteRect;
        Registry::getInstance().getComponents<Types::CollisionRect>().back() =
            collisionRect;
        SparseArray<std::size_t> &playerId =
            Registry::getInstance().getCustomSparseArray<std::size_t>(
                CustomIndex::PLAYER);
        playerId.add();
        playerId.back() = std::optional<std::size_t>(
            Registry::getInstance().getEntitiesNb() - 1);
        Registry::getInstance().getComponents<Raylib::Music>().back() = {
            musicPath,
            musicVolume};
        Registry::getInstance().getComponents<Raylib::Sound>().back() = {
            soundPath,
            soundVolume};
        Registry::getInstance().getComponents<Raylib::Text>().back() = {
            "Press space to play music, enter to play sound",
            textPos,
            fontScale,
            Raylib::DarkBlue};
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {windowCollision, init};
    }
} // namespace Systems
