/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Death systems implementation
*/

#include "DeathSystems.hpp"
#include <optional>
#include <unordered_map>
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {

    const std::function<void(std::size_t)> setPlayerAnimRectDeath =
        [](std::size_t id) {
            Registry::components<Types::AnimRect> arrAnimRect =
                Registry::getInstance().getComponents<Types::AnimRect>();

            if (arrAnimRect.exist(id)) {
                Types::AnimRect& anim = arrAnimRect[id];
                if (anim.currentRectList != Types::RectListType::DEAD) {
                    anim.changeRectList(Types::RectListType::DEAD);
                }
            }
        };

    const std::function<void(std::size_t)> setEnemyDeathFunc =
        [](std::size_t id) {
            Registry::getInstance().removeEntity(id);
        };

    // MAP FOR DEATH FUNCTIONS FOR EACH ENTITY
    const std::unordered_map<std::type_index, std::function<void(std::size_t)>>
        deathFunctions = {
            {std::type_index(typeid(Types::Player)), setPlayerAnimRectDeath},
            {std::type_index(typeid(Types::Enemy)),  setEnemyDeathFunc     },
    };

    void DeathSystems::setEntityDeathFunction(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Types::Dead> arrDead =
            Registry::getInstance().getComponents<Types::Dead>();

        std::vector<std::size_t> ids = arrDead.getExistingsId();

        for (const auto& [typeIndex, function] : deathFunctions) {
            std::vector<std::size_t> entities =
                Registry::getInstance().getEntitiesByComponents({typeIndex});
            for (std::size_t id : entities) {
                if (arrDead.exist(id)
                    && arrDead[id].deathFunction == std::nullopt) {
                    arrDead[id].deathFunction = function;
                }
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    DeathSystems::getDeathSystems()
    {
        return {setEntityDeathFunction};
    }
} // namespace Systems
