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
            static auto getInstance() -> GraphicManager &;
            void updateSystems();
            void addSystem(std::function<void(std::size_t)> /*sys*/);
            void removeSystem(std::size_t /*id*/);

            GraphicManager(GraphicManager const &) = delete;
            GraphicManager(GraphicManager const &&) = delete;
            void operator=(GraphicManager const &) = delete;
            void operator=(GraphicManager const &&) = delete;

        private:
            GraphicManager();
            ~GraphicManager();
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static GraphicManager _instance; 
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
