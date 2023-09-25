/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManagersDirector
*/

#pragma once

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
            static SystemManagersDirector _instance;
            SystemManagersDirector();

            std::list<SystemManager> _systemManagers;
    };
} // namespace Systems