#include "Logger.hpp"
#include "Systems.hpp"
#include "DeathNetworkSystem.hpp"
#include "SystemManagersDirector.hpp"
#include <csignal>

static bool isRunning = true;

static void signalHandler( int signum ) {
    Logger::info("Interrupt signal (" + std::to_string(signum) + ") received.");
    isRunning = false;
}

int main()
{
    Logger::info("Starting Server...");
    Systems::SystemManagersDirector::getInstance().addSystemManager(Systems::getECSSystems());
    Systems::SystemManagersDirector::getInstance().addSystemManager(Systems::getNetworkSystems());
    signal(SIGINT, signalHandler);

    while (isRunning) {
        Systems::SystemManagersDirector::getInstance().getSystemManager(0).updateSystems();
        Systems::SystemManagersDirector::getInstance().getSystemManager(1).updateSystems();
    }
    return 0;
}
