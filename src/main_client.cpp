/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include <boost/asio.hpp>
#include "B-luga/Logger.hpp"
#include "B-luga/Registry.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "NitworkClient.hpp"
#include "ResourcesManager.hpp"
#include "init.hpp"

constexpr int EXIT_EPITECH = 84;

int main(int /*unused*/, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#else
    Logger::setLogLevel(LogLevel::Warn);
#endif
    ResourcesManager::init(av[0]);
    initScenes();
    if (!Nitwork::NitworkClient::getInstance().startClient()) {
        return EXIT_EPITECH;
    }
    int res = Scene::SceneManager::getInstance().run();
    if (Nitwork::NitworkClient::getInstance().isRunning()) {
        Nitwork::NitworkClient::getInstance().stop();
    }
    return res;
}
