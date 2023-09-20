/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager
*/

#include "SystemManager.hpp"


namespace System {
    SystemManager::SystemManager()
    {
    }

    SystemManager::~SystemManager()
    {
    }

    void SystemManager::updateSystems()
    {
        for (auto &system : _systems) {
            system->run();
        }
    }

    void SystemManager::addSystem(std::unique_ptr<ISystem> system)
    {
        _systems.push_back(std::move(system));
    }
}
