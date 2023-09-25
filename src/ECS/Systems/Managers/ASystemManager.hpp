/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystemManager
*/

#pragma once

#include <list>
#include <memory>
#include <vector>
#include "ISystemManager.hpp"

namespace Systems {
    class ASystemManager : public ISystemManager {
        public:
            ASystemManager(const ASystemManager &)            = delete;
            ASystemManager &operator=(const ASystemManager &) = delete;
            ASystemManager(ASystemManager &&)                 = delete;
            ASystemManager &operator=(ASystemManager &&)      = delete;

            void updateSystems() override;

            void addSystem(std::function<void(std::size_t)> /*sys*/) override;

            void removeSystem(std::size_t /*id*/) override;

        protected:
            ASystemManager() = default;

            std::list<std::function<void(std::size_t)>> &getSystems();

        private:
            std::list<std::function<void(std::size_t)>> _systems;
    };
} // namespace Systems
