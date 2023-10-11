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
#include "SceneManager.hpp"

constexpr int EXIT_EPITECH = 84;

static bool checkArgs(int ac, char **av)
{
    if (ac != 3) {
        Logger::error("Usage: ./rtype_client <ip> <port>");
        return false;
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
    if (!checkArgs(ac, av)) {
        return EXIT_EPITECH;
    }
    if (!Nitwork::NitworkClient::getInstance().start(std::stoi(av[2]), 4, 60, av[1])) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkClient::getInstance().addInitMsg();
    Nitwork::NitworkClient::getInstance().addReadyMsg();
    SceneManager &sceneManager = SceneManager::getInstance();

    Logger::info("Starting Game...");
    int res = sceneManager.run();

    Nitwork::NitworkClient::getInstance().stop();
    return res;
}
