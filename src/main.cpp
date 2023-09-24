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

    Registry::components<Types::RectangleShape> arrRectShape =
    Registry::getInstance().getComponents<Types::RectangleShape>();

    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();

    Registry::components<Types::CollisionRect> arrCollisionRect =
    Registry::getInstance().getComponents<Types::CollisionRect>();

    Registry::components<Types::Sprite> arrSprite =
    Registry::getInstance().getComponents<Types::Sprite>();

    Registry::components<Types::Rect> arrRect =
    Registry::getInstance().getComponents<Types::Rect>();

    Registry::components<Types::Player> arrPlayer =
    Registry::getInstance().getComponents<Types::Player>();

    // add rectangle shape entity of 10% of the screen at the middle
    Registry::getInstance().addEntity();
    arrCollisionRect.back() = {10, 10};
    arrRectShape.back()     = {10, 10};
    arrPosition.back()      = {-5, 45};

    // add player entity test
    Registry::getInstance().addEntity();
    arrPosition.back() = {0, 0};
    arrSprite.back()   = Types::Sprite(
    {LoadTexture("assets/R-TypeSheet/r-typesheet18.gif"), 10, 20});
    arrRect.back()          = {2.0f, 5.0f, 30.5f, 25.2f};
    Types::Player myPlayer  = {true};
    arrPlayer.back()        = myPlayer;
    arrCollisionRect.back() = {10, 20};

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
