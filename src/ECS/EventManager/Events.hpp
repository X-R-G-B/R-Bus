/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Events
*/

#pragma once

#include <array>
#include <list>
#include "Raylib.hpp"

namespace Events {
    static const auto events = {
        Raylib::KeyboardKey::KB_LEFT,
        Raylib::KeyboardKey::KB_RIGHT,
        Raylib::KeyboardKey::KB_UP,
        Raylib::KeyboardKey::KB_DOWN,
    };
}
