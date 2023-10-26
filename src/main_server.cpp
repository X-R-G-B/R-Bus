#include <csignal>
#include "Logger.hpp"
#include "NitworkMainServer.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

constexpr int EXIT_EPITECH = 84;
constexpr int PORT_MIN     = 0;
constexpr int PORT_MAX     = 65535;
constexpr int SHOW_HELP    = -2;
constexpr int WRONG_ARGS   = -1;
constexpr int MAIN_SERVER  = 0;
constexpr int LOBBY_SERVER = 1;
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

static void help()
{
    Logger::info("Usage: ./r-type_server <serverType> <serverArgs>");
    Logger::info("serverType: 0 for main server, 1 for lobby server");
    Logger::info("serverArg:");
    Logger::info("\t- <port> for main server");
    Logger::info("\t- <playerNb> <serverName> <ownerIp> <ownerPort> for lobby server");
    Logger::info("Example: ./r-type_server 0 4242 (main server)");
    Logger::info("Example: ./r-type_server 1 4 serverDeXavier 172.20.10.5 4242 (lobby server)");
}

static int checkArgs(int ac, const char **av)
{
    if (ac == 1) {
        return SHOW_HELP;
    }
    const std::vector<std::string> args(av + 1, av + ac);
    if (args[0] != "0" && args[0] != "1") {
        Logger::error("Invalid server type: " + args[0]);
        return WRONG_ARGS;
    }
    if (args[0] == "0") {
        if (args.size() != 2) {
            Logger::error("Invalid number of arguments");
            return WRONG_ARGS;
        }
        if (!isNumber(args[1]) || std::stoi(args[1]) < PORT_MIN || std::stoi(args[1]) > PORT_MAX) {
            Logger::error("Invalid port: " + args[1]);
            return WRONG_ARGS;
        }
    }
    if (args[0] == "1") {
        if (args.size() != 5) {
            Logger::error("Invalid number of arguments");
            return WRONG_ARGS;
        }
        if (!isNumber(args[1]) || std::stoi(args[1]) < 1) {
            Logger::error("Invalid playerNb: " + args[1]);
            return WRONG_ARGS;
        }
        if (args[2].size() > 20) {
            Logger::error("Invalid serverName: " + args[2]);
            return WRONG_ARGS;
        }
        if (args[3].size() > 15) {
            Logger::error("Invalid ownerIp: " + args[3]);
            return WRONG_ARGS;
        }
        if (!isNumber(args[4]) || std::stoi(args[4]) < PORT_MIN || std::stoi(args[4]) > PORT_MAX) {
            Logger::error("Invalid ownerPort: " + args[4]);
            return WRONG_ARGS;
        }
    }
    return std::stoi(args[0]);
}

int main(int ac, const char **av)
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    auto serverType = checkArgs(ac, av);
    signal(SIGINT, signalHandler);
    if (serverType == MAIN_SERVER) {
        Logger::info("Starting Main Server...");
        if (!Nitwork::NitworkMainServer::getInstance().startServer(std::stoi(av[2]))) {
            return EXIT_EPITECH;
        }
        while (isRunning && Nitwork::NitworkMainServer::getInstance().isRunning()) { };
        Nitwork::NitworkMainServer::getInstance().stop();
    } else if (serverType == LOBBY_SERVER) {
        ECS::ResourcesManager::init(av[0]);
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
    } else {
        help();
        return (serverType == SHOW_HELP ? 0 : EXIT_EPITECH);
    }
    return 0;
}
