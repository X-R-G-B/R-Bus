/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager implementation
*/

#include "EventManager.hpp"
#include <algorithm>
#include "raylib.h"
#include "Events.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
EventManager EventManager::instance = EventManager();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

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
    return std::any_of(
        _activeEvents.begin(),
        _activeEvents.end(),
        [event](int e) {
            return e == event;
        });
}
