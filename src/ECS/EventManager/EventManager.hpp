/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager
*/

#pragma once

#include <list>

class EventManager {
    public:
        static auto getInstance() -> EventManager &;
        void updateEvents();
        auto checkEvent(int event) -> bool;

    private:
        EventManager() = default;

        static EventManager instance;
        std::list<int> _activeEvents;
};
