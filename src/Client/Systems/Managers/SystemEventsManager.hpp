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
            ~SystemEventsManager() override = default;
            static SystemEventsManager &getInstance();

            SystemEventsManager(SystemEventsManager const &)  = delete;
            SystemEventsManager(SystemEventsManager const &&) = delete;
            void operator=(SystemEventsManager const &)       = delete;
            void operator=(SystemEventsManager const &&)      = delete;

        private:
            SystemEventsManager();
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static SystemEventsManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
