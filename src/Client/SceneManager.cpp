/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include <iostream>
#include "ClientSystems.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

#include "CustomTypes.hpp"

constexpr int screenWidth  = 1920;
constexpr int screenHeight = 1080;

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bool SceneManager::_init             = false;
SceneManager SceneManager::_instance = SceneManager();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

static void initRaylib()
{
    Raylib::initWindow(screenWidth, screenHeight, "R-Bus");
    Raylib::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE);
    Raylib::setTargetFPS(Raylib::getMonitorRefreshRate(Raylib::getCurrentMonitor()));
    Raylib::initAudioDevice();
}

static void initSystemManagers()
{
    auto &director = Systems::SystemManagersDirector::getInstance();

    for (auto systems : Systems::getSystemsGroups()) {
        director.addSystemManager(systems);
    }
    initRaylib();
}

SceneManager &SceneManager::getInstance()
{
    if (!_init) {
        _init = true;
        initSystemManagers();
    }
    return _instance;
}

SceneManager::SceneManager() : _currentScene(Scene::MENU), _stop(false)
{
}

static void destroyRaylib()
{
    Raylib::closeAudioDevice();
    Raylib::closeWindow();
}

int SceneManager::run()
{
    auto &director = Systems::SystemManagersDirector::getInstance();

    try {
        while (!_stop && !Raylib::windowShouldClose()) {
            Raylib::beginDrawing();
            Raylib::clearBackground(Raylib::DarkGray);
            auto scene = _scenes.at(_currentScene);
            for (auto &systemManager : scene) {
                director.getSystemManager(systemManager).updateSystems();
            }
            Raylib::endDrawing();
        }
        destroyRaylib();
    } catch (std::exception &e) {
        std::cout << e.what() << std::endl;
        return ReturnValue::ERROR;
    }
    return ReturnValue::OK;
}

void SceneManager::changeScene(Scene scene)
{
    _currentScene = scene;
    Registry::getInstance().clear();
    Systems::SystemManagersDirector::getInstance().resetChanges();
}

Scene SceneManager::getCurrentScene() const
{
    return _currentScene;
}

void SceneManager::stop()
{
    _stop = true;
}
