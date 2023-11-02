/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <boost/asio.hpp>
#include "B-luga/Logger.hpp"
#include "NitworkClient.hpp"
#include "B-luga/Registry.hpp"
#include "ResourcesManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "B-luga/SceneManager.hpp"
#include "init.hpp"

constexpr int EXIT_EPITECH = 84;

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#endif
    ResourcesManager::init(av[0]);
    if (!checkArgs(ac, av)) {
        return EXIT_EPITECH;
    }
    initScenes();
    if (!Nitwork::NitworkClient::getInstance().startClient()) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkClient::getInstance().addInitMsg();
    Nitwork::NitworkClient::getInstance().addReadyMsg();
    int res = Scene::SceneManager::getInstance().run();
    if (Nitwork::NitworkClient::getInstance().isRunning()) {
        Nitwork::NitworkClient::getInstance().stop();
    }
    return res;
}
