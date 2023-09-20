/*
** SystemManager.cpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system/managers
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 10:25:26 AM 2023 brice
** Last update Thu Sep 20 2:34:27 PM 2023 brice
*/

#include "SystemManager.hpp"


namespace System {
    SystemManager::SystemManager(Registry &registry)
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

    void addSystems(std::vector<std::unique_ptr<ISystem>> systems)
    {
        for (auto &system: systems) {
            
        }
    }
}
