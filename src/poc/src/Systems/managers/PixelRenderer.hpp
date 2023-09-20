/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** PixelRenderer
*/

#pragma once

#include "ASystem.hpp"

namespace System {
    class PixelRenderer : public ASystem {
        public:
            PixelRenderer(Registry *registry);
            void run() final;
    };
}
