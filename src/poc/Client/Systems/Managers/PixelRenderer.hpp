/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** PixelRenderer
*/

#pragma once

#include "ASystemManager.hpp"

namespace Systems {
    class PixelRenderer : public ASystemManager {
        public:
            static PixelRenderer &getInstance();
            void updateSystems();
            void addSystem(std::function<void(std::size_t)>);
            void removeSystem(std::size_t);
        private:
            PixelRenderer();
            static PixelRenderer _instance;
    };
}
