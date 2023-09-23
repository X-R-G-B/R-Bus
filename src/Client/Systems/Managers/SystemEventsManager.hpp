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
            static auto getInstance() -> SystemEventsManager &;
            void updateSystems();
            void addSystem(std::function<void(std::size_t)> /*sys*/);
            void removeSystem(std::size_t /*id*/);

        private:
            SystemEventsManager();
            static SystemEventsManager _instance;
    };
} // namespace Systems
