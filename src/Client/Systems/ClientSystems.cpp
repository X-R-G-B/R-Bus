/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#include "ClientSystems.hpp"
#include "ClientNetwork.hpp"
#include "EventsSystems.hpp"
#include "GraphicsSystems.hpp"
#include "ECSSystems.hpp"

namespace Systems {
    std::array<std::vector<std::function<void(std::size_t, std::size_t)>>, 4> getSystemsGroups()
    {
        return {
            getECSSystems(),
            EventsSystems::getEventSystems(),
            GraphicSystems::getGraphicsSystems(),
            getNetworkSystems()};
    }
} // namespace Systems
