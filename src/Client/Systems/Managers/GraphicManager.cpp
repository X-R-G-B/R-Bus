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
    GraphicManager GraphicManager::_instance = GraphicManager();

    GraphicManager::GraphicManager()
    {
        const int screenWidth  = 800;
        const int screenHeight = 450;
        InitWindow(screenWidth, screenHeight, "Poc ECS");
        SetTargetFPS(60);

        addSystem(GraphicSystems::rectRenderer);
        addSystem(GraphicSystems::spriteRenderer);
    }

    GraphicManager::~GraphicManager()
    {
        CloseWindow();
    }

    GraphicManager &GraphicManager::getInstance()
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
