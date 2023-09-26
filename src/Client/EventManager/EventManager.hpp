/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager
*/

#pragma once

#include <vector>

class EventManager {
    public:
        static EventManager &getInstance();
        void updateEvents();
        bool checkEvent(int event);

    private:
        EventManager() = default;

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static EventManager instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
        std::vector<int> _activeEvents;
};
