/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** PixelRenderer
*/

#include <iostream>
#include "PixelRenderer.hpp"
#include "raylib.h"
#include "CustomTypes.hpp"

namespace System {

    PixelRenderer::PixelRenderer(Registry *registry) :
        ASystem(registry)
    {}

    void PixelRenderer::run()
    {
        int x = 0, y = 0;
        Registry::array<Pixel> arrInt = _registry->getComponents<Pixel>();

        for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
            x = begin->x;
            y = begin->y;
        }

        for (int i = 0; i < 50; i++) {
            for (int j = 0; j < 50 ; j++) {
                DrawPixel(x + i, y + j, PURPLE);
            }
        }
    }
}
