/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Raylib
*/

#include "Raylib.hpp"

namespace Raylib {
    void initWindow(int width, int height, const char *title) {
        std::cout << "initWindow" << std::endl;
        InitWindow(800, 600, "R-Type");
    }
}
