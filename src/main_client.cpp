/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "NitworkClient.hpp"

int main()
{
    Nitwork::NitworkClient::getInstance().start(4242);

    Nitwork::NitworkClient::getInstance().addInitMsg();
    while (true);
    return 0;

// #include "SceneManager.hpp"
//
// int main()
// {
//     SceneManager &sceneManager = SceneManager::getInstance();
//
//     int res = sceneManager.run();
//
//     return res;
// }
