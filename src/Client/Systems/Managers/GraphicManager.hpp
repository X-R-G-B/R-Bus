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
            static GraphicManager &getInstance();
            void updateSystems();
            void addSystem(std::function<void(std::size_t)>);
            void removeSystem(std::size_t);
        private:
            GraphicManager();
            ~GraphicManager();
            static GraphicManager _instance;
    };
}
