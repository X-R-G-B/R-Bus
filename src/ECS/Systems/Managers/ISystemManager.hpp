/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ISystemManager
*/

#pragma once

#include <functional>
#include <cstddef>

class ISystemManager {
    public:
        virtual ~ASystemManager()                         = default;
        virtual ISystemManager &getInstance() = 0;
        virtual void updateSystems() = 0;
        virtual void addSystem(std::function<void(std::size_t)> /*sys*/) = 0;
        virtual void removeSystem(std::size_t /*id*/) = 0;
}
