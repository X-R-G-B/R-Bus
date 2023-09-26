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
    Registry::components<Types::MusicStream> arrMusics =
        Registry::getInstance().getComponents<Types::MusicStream>();
    Registry::components<Types::Text> arrText =
        Registry::getInstance().getComponents<Types::Text>();

    const Types::CollisionRect playerCollisionRect = {10, 20};
    const Types::Rect playerRect                   = {2.0F, 5.0F, 30.5F, 25.2F};
    const Types::RectangleShape rectShape          = {10, 10};
    const Types::Position playerPosition           = {0, 0};
    const Types::Position squarePosition           = {-5, 45};
    const Types::Position playerTextPosition       = {40, 40};
    const Types::Text playerText                   = {
                          "Player",
                          BLACK,
                          LoadFont("assets/Fonts/soliden/SolidenTrial-Black.otf"),
                          5.5};

    // add rectangle shape entity of 10% of the screen at the middle
    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPix, sizeArrPix};
    arrSprite.back() =
        Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet10.gif")});
    arrRect.back()   = {sizeArrPix, sizeArrPix, 0, 0};
    arrSounds.back() = Types::SoundEffect("assets/Audio/Sounds/yes.ogg");

    Registry::getInstance().addEntity();
    arrPosition.back() = {sizeArrPixLarge, sizeArrPixLarge};
    arrSprite.back() =
        Types::Sprite({LoadTexture("assets/R-TypeSheet/r-typesheet11.gif")});
    arrRect.back() = {sizeArrPix, sizeArrPix, 0, 0};

    Registry::getInstance().addEntity();
    arrPixel.back()  = {sizeArrPix, sizeArrPix};
    arrMusics.back() = Types::MusicStream("assets/Audio/Musics/Title.mp3");

    // add text
    Registry::getInstance().addEntity();
    arrText.back()     = playerText;
    arrPosition.back() = playerTextPosition;

    while (true) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
