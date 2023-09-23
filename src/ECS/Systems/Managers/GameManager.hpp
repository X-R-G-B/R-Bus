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
            static GameManager _instance;
    };
} // namespace Systems
