/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector implementation
*/

#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

namespace Systems {

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    SystemManagersDirector SystemManagersDirector::_instance = SystemManagersDirector();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    SystemManagersDirector &SystemManagersDirector::getInstance()
    {
        return _instance;
    }

    SystemManager &SystemManagersDirector::getSystemManager(std::size_t id)
    {
        return _systemManagers[id];
    }

    std::size_t SystemManagersDirector::addSystemManager(
        std::vector<std::function<void(std::size_t, std::size_t)>> systems)
    {
        _systemManagers.emplace_back(systems);
        return _systemManagers.size() - 1;
    }

    void SystemManagersDirector::removeSystemManager(std::size_t id)
    {
        auto it = _systemManagers.begin();
        std::advance(it, id);
        _systemManagers.erase(it);
    }

    void SystemManagersDirector::resetChanges()
    {
        for (auto &manager : _systemManagers) {
            manager.resetChanges();
        }
    }
} // namespace Systems
