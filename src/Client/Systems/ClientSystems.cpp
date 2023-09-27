/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#include "ClientSystems.hpp"

namespace Systems {
    std::array<std::vector<std::function<void(std::size_t)>>, 3>
    getSystemsGroups()
    {
        return {
            getECSSystems(),
            EventsSystems::getEventSystems(),
            GraphicSystems::getGraphicsSystems()};
    }
} // namespace Systems
