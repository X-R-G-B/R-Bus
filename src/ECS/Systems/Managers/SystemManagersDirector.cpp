/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector implementation
*/

#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

namespace Systems {

    SystemManagersDirector SystemManagersDirector _instance = SystemManagersDirector();

    SystemManagersDirector &SystemManagersDirector::getInstance()
    {
        return _instance;
    }

    SystemManager &getSystemManager(std::size_t id)
    {
        return _systemManagers[id];
    }

    std::size_t SystemManagersDirector::addSystemManager()
    {
        _systemManagers.push_back(SystemManager());
        return _systemManagers.size() - 1;
    }

    std::size_t SystemManagersDirector::addSystemManager(std::list<std::function<void(std::size_t)>> systems)
    {
        _systemManagers.push_back(SystemManager(systems));
        return _systemManagers.size() - 1;
    }

    void SystemManagersDirector::removeSystem(std::size_t id)
    {
        auto it = _systemManagers.begin();
        std::advance(it, id);
        _systemManagers.erase(it);
    }

    SystemManagersDirector::SystemManagersDirector()
    {
        addSystemManager(ecsSystems);
    }

}