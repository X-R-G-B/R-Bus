/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystemManager
*/

#pragma once

#include <functional>
#include <list>
#include <memory>
#include <vector>

namespace Systems {
    class ASystemManager {
        public:
            virtual ~ASystemManager()              = default;
            ASystemManager(const ASystemManager &) = delete;
            ASystemManager & operator=(const ASystemManager &) = delete;
            ASystemManager(ASystemManager &&)                  = delete;
            ASystemManager & operator=(ASystemManager &&)      = delete;

        protected:
            ASystemManager() = default;

            virtual void updateSystems();

            virtual void addSystem(std::function<void(std::size_t)> /*sys*/);

            virtual void removeSystem(std::size_t /*id*/);

            std::list<std::function<void(std::size_t)>> & getSystems();

        private:
            std::list<std::function<void(std::size_t)>> _systems;
    };
} // namespace Systems
