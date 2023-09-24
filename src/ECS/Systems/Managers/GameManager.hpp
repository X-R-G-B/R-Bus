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
            ~GameManager() override = default;
            static GameManager &getInstance();

            GameManager(GameManager const &)     = delete;
            GameManager(GameManager const &&)    = delete;
            void operator=(GameManager const &)  = delete;
            void operator=(GameManager const &&) = delete;

        private:
            GameManager();
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static GameManager _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    };
} // namespace Systems
