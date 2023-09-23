/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager implementation
*/

#include "EventManager.hpp"
#include "raylib.h"
#include "Events.hpp"

EventManager EventManager::instance = EventManager();

auto EventManager::getInstance() -> EventManager &
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

auto EventManager::checkEvent(int event) -> bool
{
    for (auto activeEvent : _activeEvents) {
        if (activeEvent == event) {
            return true;
        }
    }
    return false;
}
