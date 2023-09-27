/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager implementation
*/

#include "SystemManager.hpp"

namespace Systems {
    SystemManager::SystemManager(
        std::vector<std::function<void(std::size_t)>> systems)
        : _systems(std::move(systems))
    {
    }

    void SystemManager::updateSystems()
    {
        std::size_t i = 0;

        for (auto &system : _systems) {
            system(i);
            i++;
        }
    }

    void SystemManager::addSystem(std::function<void(std::size_t)> sys)
    {
        _systems.push_back(sys);
    }

    void SystemManager::removeSystem(std::size_t id)
    {
        auto it = _systems.begin();
        std::advance(it, id);
        _systems.erase(it);
    }
} // namespace Systems
