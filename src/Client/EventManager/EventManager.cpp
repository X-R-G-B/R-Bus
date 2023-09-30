/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventManager implementation
*/

#include "EventManager.hpp"
#include <algorithm>
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
        if (Raylib::isKeyDown(event)) {
            _activeEvents.push_back(event);
        }
    }
}

bool EventManager::checkEvent(Raylib::KeyboardKey event)
{
    return std::any_of(
        _activeEvents.begin(),
        _activeEvents.end(),
        [event](Raylib::KeyboardKey e) {
            return e == event;
        });
}
