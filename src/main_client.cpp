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

    Registry::components<Types::Pixel> arrPixel =
        Registry::getInstance().getComponents<Types::Pixel>();
    Registry::components<Types::Sprite> arrSprite =
        Registry::getInstance().getComponents<Types::Sprite>();
    Registry::components<Types::Position> arrPosition =
        Registry::getInstance().getComponents<Types::Position>();
    Registry::components<Types::Rect> arrRect =
        Registry::getInstance().getComponents<Types::Rect>();
    Registry::components<Types::SoundEffect> arrSounds =
        Registry::getInstance().getComponents<Types::SoundEffect>();

    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPix, sizeArrPix};
    arrSprite.back() =
        Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrRect.back()   = {sizeArrPix, sizeArrPix, 0, 0};
    arrSounds.back() = Types::SoundEffect("assets/Audio/yes.ogg");

    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPixLarge, sizeArrPixLarge};
    arrSprite.back() =
        Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet11.gif")});
    arrRect.back() = {sizeArrPix, sizeArrPix, 0, 0};

    Registry::getInstance().addEntity();
    arrPixel.back() = {sizeArrPix, sizeArrPix};

    while (true) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
