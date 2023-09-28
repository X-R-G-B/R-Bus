/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager
*/

#pragma once

#include <vector>
#include "Raylib.hpp"

class EventManager {
    public:
        static EventManager &getInstance();
        void updateEvents();
        bool checkEvent(Raylib::KeyboardKey event);

    private:
        EventManager() = default;

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static EventManager instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
        std::vector<Raylib::KeyboardKey> _activeEvents;
};
