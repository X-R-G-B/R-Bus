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
} // namespace Systems
