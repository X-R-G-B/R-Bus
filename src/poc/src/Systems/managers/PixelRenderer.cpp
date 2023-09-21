/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** PixelRenderer
*/

#include <iostream>
#include "raylib.h"
#include "PixelRenderer.hpp"
#include "CustomTypes.hpp"

namespace Systems {
    PixelRenderer PixelRenderer::_instance = PixelRenderer();

    PixelRenderer::PixelRenderer()
    {}

    PixelRenderer &PixelRenderer::getInstance()
    {
        return _instance;
    }

    void PixelRenderer::updateSystems()
    {
        ASystemManager::updateSystems();
    }

    void PixelRenderer::addSystem(std::function<void(std::size_t)> sys)
    {
        ASystemManager::addSystem(sys);
    }

    void PixelRenderer::removeSystem(std::size_t id)
    {
        ASystemManager::removeSystem(id);
    }
}
