/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Systems implementation
*/

#include "Systems.hpp"
#include <cstddef>
#include "ECSCustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {
    void windowCollision(std::size_t /*unused*/)
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

    static void giveDamages(std::size_t firstEntity, std::size_t secondEntity)
    {
        std::optional<Types::Health> &firstEntityHealth =
            Registry::getInstance().getComponents<Types::Health>()[firstEntity];
        std::optional<Types::Dammage> &firstEntityDammage =
            Registry::getInstance()
                .getComponents<Types::Dammage>()[firstEntity];
        std::optional<Types::Health> &secondEntityHealth =
            Registry::getInstance()
                .getComponents<Types::Health>()[secondEntity];
        std::optional<Types::Dammage> &secondEntityDammage =
            Registry::getInstance()
                .getComponents<Types::Dammage>()[secondEntity];

        if (firstEntityDammage.has_value() && secondEntityHealth.has_value()) {
            secondEntityHealth->hp -= firstEntityDammage->dammage;
        }
        if (secondEntityDammage.has_value() && firstEntityHealth.has_value()) {
            firstEntityHealth->hp -= secondEntityDammage->dammage;
        }
    }

    static void checkCollisionEntity(
        std::vector<std::optional<Types::CollisionRect>>::iterator collisionIt,
        std::vector<std::optional<Types::Position>>::iterator positionIt,
        Registry::components<Types::Position> arrPosition,
        Registry::components<Types::CollisionRect> arrCollision

    )
    {
        auto tmpCollisionIt = collisionIt;
        auto tmpPositionIt  = positionIt;

        tmpPositionIt++;
        tmpCollisionIt++;
        while (tmpCollisionIt != arrCollision.end()
               && tmpPositionIt != arrPosition.end()) {
            if (tmpCollisionIt->has_value() && tmpPositionIt->has_value()) {
                if (positionIt->value().x < tmpPositionIt->value().x
                            + tmpCollisionIt->value().width
                    && positionIt->value().x + collisionIt->value().width
                        > tmpPositionIt->value().x
                    && positionIt->value().y < tmpPositionIt->value().y
                            + tmpCollisionIt->value().height
                    && positionIt->value().y + collisionIt->value().height
                        > tmpPositionIt->value().y) {
                    giveDamages(
                        std::distance(arrPosition.begin(), positionIt),
                        std::distance(arrPosition.begin(), tmpPositionIt));
                    tmpCollisionIt++;
                    tmpPositionIt++;
                } else {
                    tmpCollisionIt++;
                    tmpPositionIt++;
                }
            }
        }
    }

    void entitiesCollision(std::size_t /*unused*/)
    {
        Registry::components<Types::Position> arrPosition =
            Registry::getInstance().getComponents<Types::Position>();
        Registry::components<Types::CollisionRect> arrCollisionRect =
            Registry::getInstance().getComponents<Types::CollisionRect>();

        auto collisionIt = arrCollisionRect.begin();
        auto positionIt  = arrPosition.begin();

        while (collisionIt != arrCollisionRect.end()
               && positionIt != arrPosition.end()) {
            if (collisionIt->has_value() && positionIt->has_value()) {
                checkCollisionEntity(
                    collisionIt,
                    positionIt,
                    arrPosition,
                    arrCollisionRect);
                collisionIt++;
                positionIt++;
            }
        }
    }

    std::vector<std::function<void(std::size_t)>> getECSSystems()
    {
        return {windowCollision, entitiesCollision};
    }
} // namespace Systems
