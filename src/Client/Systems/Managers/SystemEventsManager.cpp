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
    SystemEventsManager SystemEventsManager::_instance = SystemEventsManager();

    SystemEventsManager::SystemEventsManager()
    {
        addSystem(EventsSystems::playerMovement);
    }

    SystemEventsManager &SystemEventsManager::getInstance()
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
