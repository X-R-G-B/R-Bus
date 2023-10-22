/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector implementation
*/

#include "SystemManagersDirector.hpp"
#include "ECSSystems.hpp"

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
        return _systemManagers.at(id);
    }

    void SystemManagersDirector::addSystemManager(std::size_t id,
        std::vector<std::function<void(std::size_t, std::size_t)>> systems)
    {
        if (_systemManagers.find(id) != _systemManagers.end()) {
            throw std::runtime_error("System manager already exists, id: " + std::to_string(id));
        }
        _systemManagers.insert({id, Systems::SystemManager(id, systems)});
    }

    void SystemManagersDirector::removeSystemManager(std::size_t id)
    {
        _systemManagers.erase(id);
    }

    void SystemManagersDirector::resetChanges()
    {
        for (auto &manager : _systemManagers) {
            manager.second.resetChanges();
        }
    }
} // namespace Systems
