/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <iostream>
#include "GameManager.hpp"

int main()
{
    Systems::GameManager &gameManager = Systems::GameManager::getInstance();

    while (1) {
        gameManager.updateSystems();
    }
}
