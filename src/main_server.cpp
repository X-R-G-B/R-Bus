#include <csignal>
#include <vector>
#include "Logger.hpp"
#include "NitworkMainServer.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "ServerArgsHandling.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

constexpr int EXIT_EPITECH = 84;
static bool isRunning      = true;

static void signalHandler(int signum)
{
    Logger::info("Interrupt signal (" + std::to_string(signum) + ") received.");
    isRunning = false;
    signal(SIGINT, SIG_DFL);
}

int mainMainServer(const std::vector<std::string> &av)
{
    Logger::info("Starting Main Server...");
    auto port = std::stoi(av[2]);

    if (!Nitwork::NitworkMainServer::getInstance().startServer(port)) {
        return EXIT_EPITECH;
    }
    while (isRunning && Nitwork::NitworkMainServer::getInstance().isRunning()) { };
    Nitwork::NitworkMainServer::getInstance().stop();
    return EXIT_SUCCESS;
}

int mainLobbyServer(const std::vector<std::string> &av)
{
    Logger::info("Starting Server...");
    auto nbPlayer = std::stoi(av[2]);
    auto gameType = static_cast<gameType_e>(std::stoi(av[3]));
    auto name     = av[4];
    auto ownerIp = av[5];
    auto ownerPort = std::stoi(av[6]);

    if (!Nitwork::NitworkServer::getInstance().startServer(
            nbPlayer,
            gameType,
            name,
            ownerIp,
            ownerPort)) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkServer::getInstance().addInfoLobbyMsg();
    auto &director = Systems::SystemManagersDirector::getInstance();
    std::unique_lock<std::mutex> lock(director.mutex);
    director.addSystemManager(Systems::getECSSystems());
    lock.unlock();
    while (isRunning && Nitwork::NitworkServer::getInstance().isRunning()) {
        lock.lock();
        director.getSystemManager(0).updateSystems();
        lock.unlock();
    }
    Nitwork::NitworkServer::getInstance().stop();
    return EXIT_SUCCESS;
}

int main(int ac, const char **av)
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    std::vector<std::string> args(av, av + ac);
    auto serverType = Args::ServerArgsHandling::checkArgs(ac, av);
    auto programPath = std::string(av[0]);

    signal(SIGINT, signalHandler);
    ECS::ResourcesManager::init(programPath);
    if (serverType == Args::MAIN_SERVER) {
        return mainMainServer(args);
    } else if (serverType == Args::LOBBY_SERVER) {
        return mainLobbyServer(args);
    } else {
        Args::ServerArgsHandling::help();
        return (serverType == Args::SHOW_HELP ? EXIT_SUCCESS : EXIT_EPITECH);
    }
}
