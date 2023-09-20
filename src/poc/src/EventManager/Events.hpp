/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Events
*/

#pragma once

#include <list>
#include <array>
#include "raylib.h"

namespace Events {
    static const std::array<int, 4> events = {
        KEY_LEFT,
        KEY_RIGHT,
        KEY_UP,
        KEY_DOWN
    };
}
