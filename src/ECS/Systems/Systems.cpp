/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include "CustomTypes.hpp"
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

    constexpr int playerData = 10;

    void initPlayer(std::size_t managerId, std::size_t systemId)
    {
        Registry::getInstance().addEntity();
        Registry::getInstance().getComponents<Types::Position>().back() = {
            playerData,
            playerData};
        Registry::getInstance().getComponents<Types::RectangleShape>().back() =
            {playerData, playerData};
        Registry::getInstance().getComponents<Types::CollisionRect>().back() = {
            playerData,
            playerData};
        SparseArray<std::size_t> &playerId =
            Registry::getInstance().getCustomSparseArray<std::size_t>(
                CustomIndex::PLAYER);
        playerId.add();
        playerId.back() = std::optional<std::size_t>(
            Registry::getInstance().getEntitiesNb() - 1);
        SystemManagersDirector::getInstance()
            .getSystemManager(managerId)
            .removeSystem(systemId);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getECSSystems()
    {
        return {windowCollision, initPlayer};
    }
} // namespace Systems
