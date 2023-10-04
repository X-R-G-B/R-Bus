/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "SceneManager.hpp"
#include "Logger.hpp"

int main()
{
    SceneManager &sceneManager = SceneManager::getInstance();

    Logger::info("Starting Game...");
    int res = sceneManager.run();

    return res;
}
