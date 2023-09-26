/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager
*/

#pragma once

#include <functional>
#include <list>

namespace Systems {
    class SystemManager {
        public:
            SystemManager() = default;
            SystemManager(std::list<std::function<void(std::size_t)>> systems);
            void updateSystems();
            void addSystem(std::function<void(std::size_t)> /*sys*/);
            void removeSystem(std::size_t /*id*/);

        private:
            std::list<std::function<void(std::size_t)>> _systems;
    };
} // namespace Systems
