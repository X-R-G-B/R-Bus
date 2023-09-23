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
        addSystem(EventsSystems::playerMovement);
    }

    auto SystemEventsManager::getInstance() -> SystemEventsManager &
    {
        return _instance;
    }

    void SystemEventsManager::updateSystems()
    {
        ASystemManager::updateSystems();
    }

    void SystemEventsManager::addSystem(std::function<void(std::size_t)> sys)
    {
        ASystemManager::addSystem(sys);
    }

    void SystemEventsManager::removeSystem(std::size_t id)
    {
        ASystemManager::removeSystem(id);
    }
} // namespace Systems
