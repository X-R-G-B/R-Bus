/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "SystemManager.hpp"
#include "Collison.hpp"
#include "Movement.hpp"
#include "raylib.h"
#include "PixelRenderer.hpp"
#include "CustomTypes.hpp"

void initRaylib()
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Poc ECS");
    SetTargetFPS(60);
}

int main()
{
    initRaylib();
    System::SystemManager manager;
    System::SystemManager displaySystemManager;

    Registry *registry = new Registry();
    Registry::array<int> arrInt = registry->registerComponent<int>();
    arrInt->add(4);
    arrInt->add(69);
    Registry::array<double> arrdouble = registry->registerComponent<double>();
    arrdouble->add(69.69);
    Registry::array<double> scdContainer = registry->getComponents<double>();
    for (auto begin = arrInt->begin(); begin != arrInt->end(); begin++) {
        std::cout << *begin << std::endl;
    }
    for (auto begin = scdContainer->begin(); begin != scdContainer->end(); begin++) {
        std::cout << *begin << std::endl;
    }
    Registry::array<Pixel> arrPixel = registry->registerComponent<Pixel>();
    arrPixel->add({0, 0});

    manager.addSystem(std::make_unique<System::Collison>(registry));
    manager.addSystem(std::make_unique<System::Movement>(registry));
    displaySystemManager.addSystem(std::make_unique<System::PixelRenderer>(registry));

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        manager.updateSystems();
        displaySystemManager.updateSystems();
        EndDrawing();
    }
    CloseWindow();
    return 0;
}
