/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ClientSystems
*/

#include "ClientSystems.hpp"
#include "EventsSystems.hpp"
#include "GraphicSystems.hpp"
#include "Systems.hpp"

namespace Systems {
    std::array<std::vector<std::function<void(std::size_t, std::size_t)>>, 3> getSystemsGroups()
    {
        return {getECSSystems(), EventsSystems::getEventSystems(), GraphicSystems::getGraphicsSystems()};
    }
} // namespace Systems
