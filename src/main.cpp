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

    Registry::components<Pixel> arrPixel =
    Registry::getInstance().getComponents<Pixel>();
    Registry::components<Sprite> arrSprite =
    Registry::getInstance().getComponents<Sprite>();
    Registry::components<Position> arrPosition =
    Registry::getInstance().getComponents<Position>();
    Registry::components<Rect> arrRect =
    Registry::getInstance().getComponents<Rect>();

    Registry::getInstance().addEntity();
    arrPosition.back() = {50, 50};
    arrSprite.back() =
    Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrRect.back() = {50, 50, 0, 0};

    Registry::getInstance().addEntity();
    arrPosition.back() = {100, 100};
    arrSprite.back() =
    Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet11.gif")});
    arrRect.back() = {50, 50, 0, 0};

    Registry::getInstance().addEntity();
    arrPixel.back() = {50, 50};

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
