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

static bool isNumber(const std::string &str)
{
    return std::all_of(str.begin(), str.end(), ::isdigit);
}

static bool checkArgs(int ac, const char **av)
{
    if (ac != 3) {
        Logger::error("Usage: ./r-type_server <port> <playerNb>");
        return false;
    }
    const std::vector<std::string> args(av + 1, av + ac);
    for (const auto &arg : args) {
        if (!isNumber(arg)) {
            Logger::error("Invalid argument: " + arg);
            return false;
        }
    }
    if (std::stoi(args[0]) < PORT_MIN || std::stoi(args[0]) > PORT_MAX || std::stoi(args[1]) < 1) {
        Logger::error("Invalid port or playerNb");
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
    if (!Nitwork::NitworkServer::getInstance().startServer(std::stoi(av[1]), std::stoi(av[2]))) {
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
