/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector
*/

#pragma once

#include <vector>
#include "SystemManager.hpp"

namespace Systems {
    class SystemManagersDirector {
        public:
            static SystemManagersDirector &getInstance();
            SystemManager &getSystemManager(std::size_t);
            std::size_t addSystemManager();
            std::size_t
                addSystemManager(std::list<std::function<void(std::size_t)>>);
            void removeSystem(std::size_t);

        private:
            std::vector<SystemManager> _systemManagers;

            SystemManagersDirector();

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static SystemManagersDirector _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems