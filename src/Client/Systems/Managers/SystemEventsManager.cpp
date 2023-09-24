/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemEventsManager
*/

#include "SystemEventsManager.hpp"
#include <iostream>
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"

namespace Systems {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    SystemEventsManager SystemEventsManager::_instance = SystemEventsManager();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    SystemEventsManager::SystemEventsManager()
    {
        ASystemManager::addSystem(EventsSystems::playerMovement);
    }

    SystemEventsManager &SystemEventsManager::getInstance()
    {
        return _instance;
    }
} // namespace Systems
