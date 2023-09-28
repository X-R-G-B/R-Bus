/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager implementation
*/

#include "SystemManager.hpp"

namespace Systems {

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    std::size_t SystemManager::_managerNb = 0;
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    SystemManager::SystemManager() : _id(_managerNb), _modified(false)
    {
        _managerNb += 1;
    }

    SystemManager::SystemManager(
        std::vector<std::function<void(std::size_t, std::size_t)>> systems)
        : _id(_managerNb),
          _originalSystems(std::move(systems)),
          _modified(false)
    {
        _managerNb += 1;
    }

    void SystemManager::updateSystems()
    {
        std::size_t i = 0;

        for (auto &system : getSystems()) {
            system(_id, i);
            i++;
        }
    }

    void
    SystemManager::addSystem(std::function<void(std::size_t, std::size_t)> sys)
    {
        setModifiedSystems();
        _modifiedSystems.push_back(sys);
    }

    void SystemManager::removeSystem(std::size_t id)
    {
        setModifiedSystems();
        auto it = _modifiedSystems.begin();
        std::advance(it, id);
        _modifiedSystems.erase(it);
    }

    void SystemManager::resetChanges()
    {
        _modified = false;
        _modifiedSystems.clear();
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> &
    SystemManager::getSystems()
    {
        if (_modified) {
            return _modifiedSystems;
        }
        return _originalSystems;
    }

    void SystemManager::setModifiedSystems()
    {
        _modified        = true;
        _modifiedSystems = _originalSystems;
    }
} // namespace Systems