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
        static EventManager &getInstance();
        void updateEvents();
        bool checkEvent(int event);

    private:
        EventManager() = default;

        static EventManager instance;
        std::list<int> _activeEvents;
};
