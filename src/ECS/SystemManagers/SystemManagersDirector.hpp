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

namespace Systems {
    class SystemManagersDirector {
        public:
            static SystemManagersDirector &getInstance();
            SystemManager &getSystemManager(std::size_t);
            void addSystemManager(std::size_t id, std::vector<std::function<void(std::size_t, std::size_t)>> );
            void removeSystemManager(std::size_t);
            void resetChanges();

            std::mutex mutex;

        private:
            std::map<std::size_t, SystemManager> _systemManagers;

            SystemManagersDirector() = default;

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static SystemManagersDirector _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
