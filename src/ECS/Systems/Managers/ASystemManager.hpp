/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystemManager
*/


#pragma once

#include <memory>
#include <vector>
#include <list>
#include <functional>

namespace Systems {
    class ASystemManager {
        protected:
            ASystemManager() = default;

            void updateSystems();

            void addSystem(std::function<void(std::size_t)>);

            void removeSystem(std::size_t);
    
            std::list<std::function<void(std::size_t)>> _systems;
    };
}
