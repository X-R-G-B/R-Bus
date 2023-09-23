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
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static SystemEventsManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
