/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#include "ClientSystems.hpp"
#include "ClientNetwork.hpp"
#include "EventsSystems.hpp"
#include "GraphicSystems.hpp"
#include "MenuSystems.hpp"
#include "Systems.hpp"

namespace Systems {
    std::array<std::vector<std::function<void(std::size_t, std::size_t)>>, 5> getSystemsGroups()
    {
        return {
            getECSSystems(),
            EventsSystems::getEventSystems(),
            GraphicSystems::getGraphicsSystems(),
            getNetworkSystems(),
            Menu::getMenuSystems()};
    }
} // namespace Systems
