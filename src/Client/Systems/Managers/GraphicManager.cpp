/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicManager
*/

#include "GraphicManager.hpp"
#include <iostream>
#include "raylib.h"
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"

namespace Systems {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    GraphicManager GraphicManager::_instance = GraphicManager();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    GraphicManager::GraphicManager()
    {
        const int screenWidth  = 800;
        const int screenHeight = 450;
        const int frameRate    = 60;
        InitWindow(screenWidth, screenHeight, "Poc ECS");
        SetTargetFPS(frameRate);

        addSystem(GraphicSystems::pixelRenderer);
    }

    GraphicManager::~GraphicManager()
    {
        CloseWindow();
    }

    auto GraphicManager::getInstance() -> GraphicManager &
    {
        return _instance;
    }

    void GraphicManager::updateSystems()
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ASystemManager::updateSystems();
        EndDrawing();
    }

    void GraphicManager::addSystem(std::function<void(std::size_t)> sys)
    {
        ASystemManager::addSystem(sys);
    }

    void GraphicManager::removeSystem(std::size_t id)
    {
        ASystemManager::removeSystem(id);
    }
} // namespace Systems
