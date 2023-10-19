/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Custom Types
*/

#pragma once

#include <string>
#include <vector>
#include "ECSCustomTypes.hpp"
#ifdef CLIENT
    #include "Raylib.hpp"
#endif

namespace Types {

    struct FontSize {
            float fsz;
    };

    struct Origin {
            float x;
            float y;
    };

    struct Rotation {
            float rotate;
    };

    struct Scale {
            float size;
    };

#ifdef CLIENT
    struct Color {
            Raylib::Color color;
    };
#endif

} // namespace Types

#include "AnimRect.hpp"
