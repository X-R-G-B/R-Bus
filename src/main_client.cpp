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

    const Types::CollisionRect playerCollisionRect = {10, 20};
    const Types::Rect playerRect                   = {2.0F, 5.0F, 30.5F, 25.2F};
    const Types::RectangleShape rectShape          = {10, 10};
    const Types::Position playerPosition           = {0, 0};
    const Types::Position squarePosition           = {-5, 45};
    const Types::Sprite playerSprite               = {
                      LoadTexture("assets/R-TypeSheet/r-typesheet18.gif"),
                      10,
                      20};

    // add rectangle shape entity of 10% of the screen at the middle
    Registry::getInstance().addEntity();
    arrRectShape.back() = rectShape;
    arrPosition.back()  = squarePosition;

    // add player entity test
    Registry::getInstance().addEntity();
    arrPosition.back()      = playerPosition;
    arrSprite.back()        = playerSprite;
    arrRect.back()          = playerRect;
    Types::Player myPlayer  = {true};
    arrPlayer.back()        = myPlayer;
    arrCollisionRect.back() = playerCollisionRect;

    while (true) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
