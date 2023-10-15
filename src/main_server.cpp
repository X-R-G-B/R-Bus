#include <csignal>
#include "Logger.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

constexpr int EXIT_EPITECH = 84;
constexpr int PORT_MIN     = 0;
constexpr int PORT_MAX     = 65535;
static bool isRunning      = true;

static void signalHandler(int signum)
{
    Logger::info("Interrupt signal (" + std::to_string(signum) + ") received.");
    isRunning = false;
    signal(SIGINT, SIG_DFL);
}

static bool checkArgs(int ac, const char **av)
{
    if (ac != 2) {
        Logger::error("Usage: ./r-type_server <port>");
        return false;
    }
    if (av[1][0] == '\0') {
        Logger::error("Invalid ip");
        return false;
    }
    for (int i = 0; av[1][i] != '\0'; i++) {
        if (av[1][i] < '0' || av[1][i] > '9') {
            Logger::error("Invalid port");
            return false;
        }
    }
    if (std::stoi(av[1]) < PORT_MIN || std::stoi(av[1]) > PORT_MAX) {
        Logger::error("Invalid port");
        return false;
    }
    return true;
}

int main(int ac, const char **av)
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    ECS::ResourcesManager::init(av[0]);
    if (!checkArgs(ac, av)) {
        return EXIT_EPITECH;
    }
    Logger::info("Starting Server...");
    if (!Nitwork::NitworkServer::getInstance().start(std::stoi(av[1]))) {
        return EXIT_EPITECH;
    }
    Systems::SystemManagersDirector::getInstance().addSystemManager(Systems::getECSSystems());
    signal(SIGINT, signalHandler);

    while (isRunning && Nitwork::NitworkServer::getInstance().isRunning()) {
        Systems::SystemManagersDirector::getInstance().getSystemManager(0).updateSystems();
    }
    Nitwork::NitworkServer::getInstance().stop();
    return 0;
}
