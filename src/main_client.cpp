/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "SceneManager.hpp"

int main()
{
    SceneManager &sceneManager = SceneManager::getInstance();

    int res = sceneManager.run();

    return res;
}
