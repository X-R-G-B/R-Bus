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

void uglyCode()
{
    constexpr int sizeArrPix         = 50;
    constexpr int sizeArrPixLarge    = 100;

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
}

int main()
{
    Systems::SystemManagersDirector &director = Systems::SystemManagersDirector::getInstance();
    director.addSystemManager(Systems::EventsSystems::eventSystems);
    director.addSystemManager(Systems::GraphicSystems::graphicSystems);
    std::list<std::size_t> managersIds = {1, 0, 2};

    uglyCode();

    while (true) {
        for (auto id : managersIds) {
            director.getSystemManager(id).updateSystems();
        }
    }
}
