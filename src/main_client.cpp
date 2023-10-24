/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "Logger.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "SystemManagersDirector.hpp"
#include "SceneManager.hpp"
#include "init.hpp"

constexpr int EXIT_EPITECH = 84;

static bool isNumber(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

static bool checkArgs(int ac, const char **av)
{
    if (ac != 3) {
        Logger::error("Usage: ./r-type_client <ip> <port>");
        return false;
    }
    const std::vector<std::string> args(av + 1, av + ac);
    if (args[0].empty()) {
        Logger::error("Invalid ip");
        return false;
    }
    if (!isNumber(args[1]) || std::stoi(args[1]) < 0 || std::stoi(args[1]) > 65535) {
        Logger::error("Invalid port");
        return false;
    }
    return true;
}

int main(int ac, const char **av)
{
#ifndef NDEBUG
    Logger::setLogLevel(LogLevel::Debug);
#endif
    ECS::ResourcesManager::init(av[0]);
    if (!checkArgs(ac, av)) {
        return EXIT_EPITECH;
    }
    initScenes();
    if (!Nitwork::NitworkClient::getInstance()
             .startClient(std::stoi(av[2]), av[1], DEFAULT_THREAD_NB, TICKS_PER_SECOND)) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkClient::getInstance().addInitMsg();
    Nitwork::NitworkClient::getInstance().addReadyMsg();
    int res = Scene::SceneManager::getInstance().run();
    Nitwork::NitworkClient::getInstance().stop();
    return res;
}
