/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "GameManager.hpp"
#include "GraphicManager.hpp"
#include "SystemEventsManager.hpp"
#include "CustomTypes.hpp"
#include "Registry.hpp"
#include "raylib.h"

int main()
{
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager = Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager = Systems::SystemEventsManager::getInstance();

    Registry::components<Pixel> arrPixel = Registry::getInstance().registerComponent<Pixel>();
    Registry::components<Sprite> arrSprite = Registry::getInstance().registerComponent<Sprite>();
    Registry::components<Position> arrPosition = Registry::getInstance().registerComponent<Position>();
    Registry::components<Rect> arrRect = Registry::getInstance().registerComponent<Rect>();
    arrSprite.add({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrPosition.add({50, 50});
    for (auto begin = arrSprite.begin(); begin != arrSprite.end(); begin++) {
        arrRect.add({begin->sprite.width, begin->sprite.height,0, 0});
    }
    arrPixel.add({20, 20});

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
