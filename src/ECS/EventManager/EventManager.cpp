/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager implementation
*/

#include "raylib.h"
#include "EventManager.hpp"
#include "Events.hpp"

EventManager EventManager::instance = EventManager();

EventManager &EventManager::getInstance()
{
    return instance;
}

void EventManager::updateEvents()
{
    _activeEvents.clear();
    for (auto event : Events::events) {
        if (IsKeyDown(event)) {
            _activeEvents.push_back(event);
        }
    }
}

bool EventManager::checkEvent(int event)
{
    for (auto activeEvent : _activeEvents) {
        if (activeEvent == event) {
            return true;
        }
    }
    return false;
}
