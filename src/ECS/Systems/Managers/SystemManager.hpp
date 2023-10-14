/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager
*/

#pragma once

#include <functional>
#include <vector>
#include <mutex>

namespace Systems {
    class SystemManager {
        public:
            SystemManager();
            SystemManager(std::vector<std::function<void(std::size_t, std::size_t)>> systems);
            void updateSystems();
            void addSystem(std::function<void(std::size_t, std::size_t)> /*sys*/);
            void removeSystem(std::size_t /*id*/);
            void resetChanges();

        private:
            std::vector<std::function<void(std::size_t, std::size_t)>>& getSystems();

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static std::size_t _managerNb;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

            std::size_t _id;
            std::vector<std::size_t> _toRemove;
            std::vector<std::function<void(std::size_t, std::size_t)>> _originalSystems;
            std::vector<std::function<void(std::size_t, std::size_t)>> _modifiedSystems;
            bool _modified;
    };
} // namespace Systems
