/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystemManager implementation
*/

#include "GameManager.hpp"
#include "Systems.hpp"

namespace Systems {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    GameManager GameManager::_instance = GameManager();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    GameManager::GameManager()
    {
        ASystemManager::addSystem(Systems::collision);
    }

    GameManager &GameManager::getInstance()
    {
        return _instance;
    }

    void GameManager::updateSystems()
    {
        ASystemManager::updateSystems();
    }

    void GameManager::addSystem(std::function<void(std::size_t)> sys)
    {
        ASystemManager::addSystem(sys);
    }

    void GameManager::removeSystem(std::size_t id)
    {
        ASystemManager::removeSystem(id);
    }
} // namespace Systems
