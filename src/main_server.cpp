#include <csignal>
#include "Logger.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

static bool isRunning = true;

static void signalHandler(int signum)
{
    Logger::info("Interrupt signal (" + std::to_string(signum) + ") received.");
    isRunning = false;
}

int main()
{
    Logger::info("Starting Server...");
    Systems::SystemManagersDirector::getInstance().addSystemManager(Systems::getECSSystems());
    signal(SIGINT, signalHandler);

    while (isRunning) {
        Systems::SystemManagersDirector::getInstance().getSystemManager(0).updateSystems();
    }
    return 0;
}
