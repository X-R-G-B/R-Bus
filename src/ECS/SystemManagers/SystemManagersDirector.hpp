/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector
*/

#pragma once

#include <vector>
#include <map>
#include "SystemManager.hpp"
#include "SystemManagersDirector.hpp"
#include "ECSSystems.hpp"

namespace Systems {
    class SystemManagersDirector {
        public:
            static SystemManagersDirector &getInstance()
            {
                static SystemManagersDirector instance;
                return instance;
            }

            SystemManager &getSystemManager(std::size_t id)
            {
                return _systemManagers.at(id);
            }

            void addSystemManager(std::size_t id, std::vector<std::function<void(std::size_t, std::size_t)>> systems)
            {
                if (_systemManagers.find(id) != _systemManagers.end()) {
                    throw std::runtime_error("System manager already exists, id: " + std::to_string(id));
                }
                _systemManagers.insert({id, Systems::SystemManager(id, systems)});
            }

            void removeSystemManager(std::size_t id)
            {
                _systemManagers.erase(id);
            }

            void resetChanges()
            {
                for (auto &manager : _systemManagers) {
                    manager.second.resetChanges();
                }
            }

            std::mutex mutex;

        private:
            std::map<std::size_t, SystemManager> _systemManagers;

            SystemManagersDirector() = default;

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static SystemManagersDirector _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
