/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicManager
*/

#include "GraphicManager.hpp"
#include <iostream>
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"
#include "Raylib.hpp"

namespace Systems {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    GraphicManager GraphicManager::_instance = GraphicManager();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    GraphicManager::GraphicManager()
    {
        const int screenWidth  = 800;
        const int screenHeight = 450;
        const int frameRate    = 60;
        Raylib::initWindow(screenWidth, screenHeight, "R-Type");
        Raylib::setTargetFPS(frameRate);

        addSystem(GraphicSystems::rectRenderer);
        addSystem(GraphicSystems::spriteRenderer);
    }

    GraphicManager::~GraphicManager()
    {
        Raylib::closeWindow();
    }

    GraphicManager &GraphicManager::getInstance()
    {
        return _instance;
    }

    void GraphicManager::updateSystems()
    {
        Raylib::beginDrawing();
        Raylib::clearBackground(Raylib::Black);
        ASystemManager::updateSystems();
        Raylib::endDrawing();
    }
} // namespace Systems
