/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager
*/

#pragma once

#include <memory>
#include <vector>
#include "ISystem.hpp"

namespace System {
    class SystemManager {
        public:
            SystemManager();
            ~SystemManager();

            void updateSystems();

            void addSystem(std::unique_ptr<ISystem> system);
    
        protected:
            std::vector<std::unique_ptr<ISystem>> _systems;
    };
}
