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
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager =
    Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager =
    Systems::SystemEventsManager::getInstance();

    Registry::components<Pixel> arrPixel =
    Registry::getInstance().getComponents<Pixel>();
    Registry::getInstance().addEntity();
    arrPixel.back() = {20, 20};
    Registry::getInstance().addEntity();
    arrPixel.back() = {100, 20};
    Registry::getInstance().removeEntity(0);

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
