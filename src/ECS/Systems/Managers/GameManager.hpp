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
            static GameManager &getInstance();
            void updateSystems();
            void addSystem(std::function<void(std::size_t)>);
            void removeSystem(std::size_t);
        private:
            GameManager();
            static GameManager _instance;
    };
}
