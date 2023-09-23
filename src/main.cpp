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

int main()
{
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();
    Systems::GraphicManager &graphicManager =
    Systems::GraphicManager::getInstance();
    Systems::SystemEventsManager &systemEventsManager =
    Systems::SystemEventsManager::getInstance();

    // We create a Rectangle of size 10% at the middle of the screen
    Registry::components<Types::RectangleShape> arrRect =
    Registry::getInstance().getComponents<Types::RectangleShape>();

    Registry::components<Types::Position> arrPosition =
    Registry::getInstance().getComponents<Types::Position>();

    Registry::components<Types::CollisionRect> arrCollisionRect =
    Registry::getInstance().getComponents<Types::CollisionRect>();
    Registry::getInstance().addEntity();

    arrCollisionRect.back() = {10, 10};
    arrRect.back()          = {10, 10};
    arrPosition.back()      = {50, 50};

    while (1) {
        systemEventsManager.updateSystems();
        gameManager.updateSystems();
        graphicManager.updateSystems();
    }
}
