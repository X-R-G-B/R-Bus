/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemEventsManager
*/

#pragma once

#include "ASystemManager.hpp"

namespace Systems {
    class SystemEventsManager : public ASystemManager {
        public:
            static SystemEventsManager &getInstance();
            void updateSystems();
            void addSystem(std::function<void(std::size_t)>);
            void removeSystem(std::size_t);

        private:
            SystemEventsManager();
            static SystemEventsManager _instance;
    };
} // namespace Systems
