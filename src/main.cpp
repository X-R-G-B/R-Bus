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

int main()
{
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager = Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager = Systems::SystemEventsManager::getInstance();

    Registry::array<Pixel> arrPixel = Registry::getInstance().registerComponent<Pixel>();
    arrPixel.add({20, 20});

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
