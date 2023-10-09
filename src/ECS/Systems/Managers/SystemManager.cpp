/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager implementation
*/

#include <algorithm>
#include "SystemManager.hpp"

namespace Systems {

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    std::size_t SystemManager::_managerNb = 0;
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    SystemManager::SystemManager() : _id(_managerNb), _modified(false)
    {
        _managerNb += 1;
    }

    SystemManager::SystemManager(std::vector<std::function<void(std::size_t, std::size_t)>> systems)
        : _id(_managerNb),
          _originalSystems(std::move(systems)),
          _modifiedSystems(_originalSystems),
          _modified(false)
    {
        _managerNb += 1;
    }

    void SystemManager::updateSystems()
    {
        std::size_t i = 0;
        std::size_t decrease = 0;

        _toRemove.clear();
        for (auto &system : getSystems()) {
            system(_id, i);
            i++;
        }
        std::sort(_toRemove.begin(), _toRemove.end());
        for (auto &id : _toRemove) {
            auto it = _modifiedSystems.begin();
            std::advance(it, id);
            _modifiedSystems.erase(it - decrease);
            decrease++;
        }
    }

    void SystemManager::addSystem(std::function<void(std::size_t, std::size_t)> sys)
    {
        if (!_modified) {
            _modified = true;
        }
        _modifiedSystems.push_back(sys);
    }

    void SystemManager::removeSystem(std::size_t id)
    {
        if (!_modified) {
            _modified = true;
        }
        _toRemove.push_back(id);
    }

    void SystemManager::resetChanges()
    {
        _modified = false;
        _modifiedSystems = _originalSystems;
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> &SystemManager::getSystems()
    {
        if (_modified) {
            return _modifiedSystems;
        }
        return _originalSystems;
    }

} // namespace Systems
