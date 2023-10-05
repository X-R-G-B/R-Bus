/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

//#include "NitworkClient.hpp"
//
// int main()
//{
//     const int port = 4242;
//     Nitwork::NitworkClient::getInstance().start(port);
//
//     Nitwork::NitworkClient::getInstance().addInitMsg();
//     Nitwork::NitworkClient::getInstance().addInitMsg();
//     Nitwork::NitworkClient::getInstance().addInitMsg();
//     Nitwork::NitworkClient::getInstance().addInitMsg();
//     Nitwork::NitworkClient::getInstance().addReadyMsg();
//     while (true) {};
//     return 0;
// }

#include "Logger.hpp"
#include "SceneManager.hpp"

int main()
{
    SceneManager &sceneManager = SceneManager::getInstance();

    Logger::info("Starting Game...");
    int res = sceneManager.run();

    return res;
}
