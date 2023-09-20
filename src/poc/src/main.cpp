/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "raylib.h"
#include "SystemManager.hpp"
#include "Collison.hpp"
#include "Movement.hpp"
#include "EventManager.hpp"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - keyboard input");

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    SetTargetFPS(60);
    EventManager &eventManager = EventManager::getInstance();

    while (1) {
        eventManager.updateEvents();
        if (eventManager.checkEvent(KEY_LEFT)) {
            std::cout << "yes yes yes" << std::endl;
        }
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("move the ball with arrow keys", 10, 10, 20, DARKGRAY);

            DrawCircleV(ballPosition, 50, MAROON);

        EndDrawing();
    }
    CloseWindow();
    return 0;
}
