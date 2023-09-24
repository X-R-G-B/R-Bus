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
    const int sizeArrPix              = 50;
    const int sizeArrPixLarge         = 100;
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager =
        Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager =
        Systems::SystemEventsManager::getInstance();

    Registry::components<Pixel> arrPixel =
        Registry::getInstance().getComponents<Pixel>();
    Registry::components<Sprite> arrSprite =
        Registry::getInstance().getComponents<Sprite>();
    Registry::components<Position> arrPosition =
        Registry::getInstance().getComponents<Position>();
    Registry::components<Rect> arrRect =
        Registry::getInstance().getComponents<Rect>();

    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPix, sizeArrPix};
    arrSprite.back() =
        Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrRect.back() = {sizeArrPix, sizeArrPix, 0, 0};

    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPixLarge, sizeArrPixLarge};
    arrSprite.back() =
        Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet11.gif")});
    arrRect.back() = {sizeArrPix, sizeArrPix, 0, 0};

    Registry::getInstance().addEntity();
    arrPixel.back() = {sizeArrPix, sizeArrPix};

    while (true) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
