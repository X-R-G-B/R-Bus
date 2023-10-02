/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Death systems implementation
*/

#include "DeathSystems.hpp"
#include <iostream>
#include <optional>
#include <unordered_map>
#include "CustomTypes.hpp"
#include "Registry.hpp"

namespace Systems {

    // REPLACE BY YOUR DEATH FUNCTION

    const std::function<void(std::size_t)> testFuncPlayer = [](std::size_t id) {
        std::cout << "Player " << id << " is dead" << std::endl;
    };

    // MAP FOR DEATH FUNCTIONS
    const std::unordered_map<std::type_index, std::function<void(std::size_t)>>
        deathFunctions = {
            {std::type_index(typeid(Types::Player)), testFuncPlayer},
    };

    static void addDeathFunction(
        Registry::components<Types::Dead> &arrDead,
        Registry::components<Types::Player> &arrPlayer,
        std::size_t id)
    {
        try {
            if (arrPlayer.exist(id)) {
                arrDead[id].deathFunction =
                    deathFunctions.at(std::type_index(typeid(Types::Player)));
            }
        } catch (std::out_of_range &) {
            std::cerr << "No death function for this entity" << std::endl;
        }
    }

    void DeathSystems::setEntityDeathFunction(
        std::size_t /*unused*/,
        std::size_t /*unused*/)
    {
        Registry::components<Types::Dead> arrDead =
            Registry::getInstance().getComponents<Types::Dead>();
        Registry::components<Types::Player> arrPlayer =
            Registry::getInstance().getComponents<Types::Player>();

        std::vector<std::size_t> ids = arrDead.getExistingsId();

        for (auto itIds = ids.begin(); itIds != ids.end(); itIds++) {
            if (arrDead[*itIds].deathFunction == std::nullopt) {
                addDeathFunction(arrDead, arrPlayer, *itIds);
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>>
    DeathSystems::getDeathSystems()
    {
        return {setEntityDeathFunction};
    }
} // namespace Systems
