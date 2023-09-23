/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "raylib.h"
#include "CustomTypes.hpp"
#include "GameManager.hpp"
#include "GraphicManager.hpp"
#include "Registry.hpp"
#include "SystemEventsManager.hpp"

int main()
{
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager =
    Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager =
    Systems::SystemEventsManager::getInstance();

    // We create a Rectangle of size 10% at the middle of the screen
    Registry::components<Types::RectangleShape> arrRectShape =
    Registry::getInstance().getComponents<Types::RectangleShape>();

    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();

    Registry::components<Types::CollisionRect> arrCollisionRect =
    Registry::getInstance().getComponents<Types::CollisionRect>();
    Registry::getInstance().addEntity();

    arrCollisionRect.back() = {10, 10};
    arrRectShape.back()          = {10, 10};
    arrPosition.back()      = {50, 50};

    Registry::components<Types::Sprite> arrSprite =
    Registry::getInstance().getComponents<Types::Sprite>();
    Registry::components<Types::Rect> arrRect =
    Registry::getInstance().getComponents<Types::Rect>();

    Registry::getInstance().addEntity();
    arrPosition.back() = {50, 50};
    arrSprite.back() =
    Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrRect.back() = {50, 50, 0, 0};

    Registry::getInstance().addEntity();
    arrPosition.back() = {100, 100};
    arrSprite.back() =
    Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet11.gif")});
    arrRect.back() = {50, 50, 0, 0};

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
