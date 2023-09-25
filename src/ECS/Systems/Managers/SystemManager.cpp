/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager implementation
*/

#include "ASystemManager.hpp"

namespace Systems {
    SystemManager::SystemManager()
    {
    }

    SystemManager::SystemManager(
        std::list<std::function<void(std::size_t)>> systems)
        : _systems(systems)
    {
    }

    void ASystemManager::updateSystems()
    {
        std::size_t i = 0;

        for (auto &system : _systems) {
            system(i);
            i++;
        }
    }

    void ASystemManager::addSystem(std::function<void(std::size_t)> sys)
    {
        _systems.push_back(sys);
    }

    void ASystemManager::removeSystem(std::size_t id)
    {
        auto it = _systems.begin();
        std::advance(it, id);
        _systems.erase(it);
    }
} // namespace Systems
