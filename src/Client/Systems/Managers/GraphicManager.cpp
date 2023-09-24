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
        InitAudioDevice();
        SetTargetFPS(frameRate);

        ASystemManager::addSystem(GraphicSystems::pixelRenderer);
        ASystemManager::addSystem(GraphicSystems::spriteRenderer);
        ASystemManager::addSystem(GraphicSystems::soundEffectPlayer);
        ASystemManager::addSystem(EventsSystems::playerMovement);
        ASystemManager::addSystem(GraphicSystems::musicPlayer);
    }

    GraphicManager::~GraphicManager()
    {
        CloseAudioDevice();
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
} // namespace Systems
