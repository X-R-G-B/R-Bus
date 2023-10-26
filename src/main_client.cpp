/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** main
*/

#include "Logger.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"
#include "ResourcesManager.hpp"
#include "SceneManager.hpp"

constexpr int EXIT_EPITECH = 84;

int main(int ac /*unused*/, const char **av)
{
#ifndef NDEBUG
    Registry::getInstance().getLogger().setLogLevel(Logger::LogLevel::Debug);
#endif
    ECS::ResourcesManager::init(av[0]);
    auto &sceneManager = Scene::SceneManager::getInstance();
    int res            = sceneManager.run();
    if (Nitwork::NitworkClient::getInstance().isRunning()) {
        Nitwork::NitworkClient::getInstance().stop();
    }
    return res;
}
