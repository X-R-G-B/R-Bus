#include <csignal>
#include "Logger.hpp"
#include "NitworkServer.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"
#include "Registry.hpp"

static bool isRunning = true;

static void signalHandler(int signum)
{
    Logger::info("Interrupt signal (" + std::to_string(signum) + ") received.");
    isRunning = false;
    signal(SIGINT, SIG_DFL);
}

int main()
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    Logger::info("Starting Server...");
    Nitwork::NitworkServer::getInstance().start(4242);
    Systems::SystemManagersDirector::getInstance().addSystemManager(Systems::getECSSystems());
    signal(SIGINT, signalHandler);

    while (isRunning) {
        Systems::SystemManagersDirector::getInstance().getSystemManager(0).updateSystems();
    }
    Nitwork::NitworkServer::getInstance().stop();
    return 0;
}
