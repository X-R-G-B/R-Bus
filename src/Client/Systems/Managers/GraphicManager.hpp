/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicManager
*/

#pragma once

#include "ASystemManager.hpp"

namespace Systems {
    class GraphicManager : public ASystemManager {
        public:
            ~GraphicManager() final;
            static GraphicManager &getInstance();
            void updateSystems() final;

            GraphicManager(GraphicManager const &)  = delete;
            GraphicManager(GraphicManager const &&) = delete;
            void operator=(GraphicManager const &)  = delete;
            void operator=(GraphicManager const &&) = delete;

        private:
            GraphicManager();
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static GraphicManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
