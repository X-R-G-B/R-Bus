/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

// #include "NitworkClient.hpp"
//
//  int main()
//{
//      const int port = 4242;
//      Nitwork::NitworkClient::getInstance().start(port);
//
//      Nitwork::NitworkClient::getInstance().addInitMsg();
//      Nitwork::NitworkClient::getInstance().addInitMsg();
//      Nitwork::NitworkClient::getInstance().addInitMsg();
//      Nitwork::NitworkClient::getInstance().addInitMsg();
//      Nitwork::NitworkClient::getInstance().addReadyMsg();
//      while (true) {};
//      return 0;
//  }

#include <cstddef>
#include "Logger.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "SceneManager.hpp"

constexpr int EXIT_EPITECH = 84;

static bool checkArgs(int ac, char **av)
{
    if (ac != 3) {
        Logger::error("Usage: ./r-type_client <ip> <port>");
        return false;
    }
    for (int i = 0; av[2][i] != '\0'; i++) {
        if (av[2][i] < '0' || av[2][i] > '9') {
            Logger::error("Invalid port");
            return false;
        }
    }
    if (std::stoi(av[2]) < 0 || std::stoi(av[2]) > 65535) {
        Logger::error("Invalid port");
        return false;
    }
    if (av[1][0] == '\0') {
        Logger::error("Invalid ip");
        return false;
    }
    return true;
}

int main(int ac, char **av)
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    ECS::ResourcesManager::init(av[0]);
    if (!checkArgs(ac, av)) {
        return EXIT_EPITECH;
    }
    auto &sceneManager = Scene::SceneManager::getInstance();
    if (!Nitwork::NitworkClient::getInstance()
             .start(std::stoi(av[2]), DEFAULT_THREAD_NB, TICKS_PER_SECOND, av[1])) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkClient::getInstance().addInitMsg();
    Nitwork::NitworkClient::getInstance().addReadyMsg();
    int res = sceneManager.run();
    Nitwork::NitworkClient::getInstance().stop();
    return res;
}
