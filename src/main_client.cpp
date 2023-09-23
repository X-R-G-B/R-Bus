/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "CustomTypes.hpp"
#include "GameManager.hpp"
#include "GraphicManager.hpp"
#include "Registry.hpp"
#include "SystemEventsManager.hpp"

auto main() -> int
{
    const int sizeArrPix = 20;
    const int sizeArrPixLarge = 100;
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager =
    Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager =
    Systems::SystemEventsManager::getInstance();

    Registry::components<Pixel> arrPixel =
    Registry::getInstance().getComponents<Pixel>();
    Registry::getInstance().addEntity();
    arrPixel.back() = {sizeArrPix, sizeArrPix};
    Registry::getInstance().addEntity();
    arrPixel.back() = {sizeArrPixLarge, sizeArrPix};
    Registry::getInstance().removeEntity(0);

    while (true) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
