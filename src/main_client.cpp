/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "raylib.h"
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

void beautyCode()
{
    Registry &registry = Registry::getInstance();
    registry.addEntity();
    registry.getComponents<Types::RectangleShape>().back() = {10, 10};
    registry.getComponents<Types::Position>().back() = {-5, 45};
    registry.addEntity();
    registry.getComponents<Types::Position>().back() = {0, 0};
    registry.getComponents<Types::CollisionRect>().back() = {10, 20};
    std::cout << "before Sprite" << std::endl;
    Registry::components<Types::Sprite> sprites = registry.getComponents<Types::Sprite>();
    std::cout << "mid" << std::endl;
    sprites.back() = {LoadTexture("./assets/R-TypeSheet/r-typesheet1.gif"), 10, 20};
    std::cout << "after sprite" << std::endl;
    registry.getComponents<Types::Rect>().back() = {2.0F, 5.0F, 30.5F, 25.2F};
    registry.getComponents<Types::Player>().back() = Types::Player(true);
}

const int screenWidth = 1920;
const int screenHeight = 1080;
const int fps = 60;

int main()
{
    Systems::SystemManagersDirector &director = Systems::SystemManagersDirector::getInstance();
    director.addSystemManager(Systems::EventsSystems::eventSystems);
    director.addSystemManager(Systems::GraphicSystems::graphicSystems);
    std::list<std::size_t> managersIds = {1, 0, 2};

    InitWindow(screenWidth, screenHeight, "raylib [textures] examples - texture source and destination rectangles");
    SetTargetFPS(fps);
    beautyCode();
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        for (auto id : managersIds) {
            director.getSystemManager(id).updateSystems();
        }
        EndDrawing();
    }
    CloseWindow();
}
