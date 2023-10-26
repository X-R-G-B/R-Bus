#include <csignal>
#include "Logger.hpp"
#include "MerverArgsHandling.hpp"
#include "NitworkMainServer.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
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
    if (!Nitwork::NitworkMainServer::getInstance().startServer(std::stoi(av[2]))) {
        return EXIT_EPITECH;
    }
    while (isRunning && Nitwork::NitworkMainServer::getInstance().isRunning()) { };
    Nitwork::NitworkMainServer::getInstance().stop();
    return EXIT_SUCCESS;
}

int mainLoobyServer(const std::vector<std::string> &av)
{
    Logger::info("Starting Server...");
    if (!Nitwork::NitworkServer::getInstance()
             .startServer(std::stoi(av[2]), av[3], av[4], std::stoi(av[5]))) {
        return EXIT_EPITECH;
    }
    Nitwork::NitworkServer::getInstance().addInfoMsg();
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

    signal(SIGINT, signalHandler);
    ECS::ResourcesManager::init(av[0]);
    if (serverType == Args::MAIN_SERVER) {
        return mainMainServer(args);
    } else if (serverType == Args::LOBBY_SERVER) {
        return mainLoobyServer(args);
    } else {
        Args::ServerArgsHandling::help();
        return (serverType == Args::SHOW_HELP ? EXIT_SUCCESS : EXIT_EPITECH);
    }
}
