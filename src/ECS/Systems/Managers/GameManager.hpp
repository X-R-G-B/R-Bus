/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystemManager implementation
*/

#pragma once

#include "ASystemManager.hpp"

namespace Systems {
    class GameManager : public ASystemManager {
        public:
            static auto getInstance() -> GameManager &;
            void updateSystems();
            void addSystem(std::function<void(std::size_t)> /*sys*/);
            void removeSystem(std::size_t /*id*/);

        private:
            GameManager();
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static GameManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
