/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Events
*/

#pragma once

#include <list>
#include "raylib.h"

namespace Events {
    static const std::list<int> events = {
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN
    };
}
