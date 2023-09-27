/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "raylib.h"
#include "ClientSystems.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

constexpr int screenWidth  = 1920;
constexpr int screenHeight = 1080;
constexpr int fps          = 60;

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bool SceneManager::_init             = false;
SceneManager SceneManager::_instance = SceneManager();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

SceneManager &SceneManager::getInstance()
{
    if (!_init) {
        _init = true;
        _instance.init();
    }
    return _instance;
}

SceneManager::SceneManager() : _currentScene(Scene::MAIN_GAME), _stop(false)
{
}

static void initRaylib()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "R-Bus");
    SetTargetFPS(fps);
    InitAudioDevice();
}

static void destroyRaylib()
{
    CloseAudioDevice();
    CloseWindow();
}

void SceneManager::init()
{
    auto &director = Systems::SystemManagersDirector::getInstance();

    for (auto systems : Systems::getSystemsGroups()) {
        director.addSystemManager(systems);
    }
    initRaylib();
    while (!_stop && !WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        auto scene = _scenes.at(_currentScene);
        for (auto &systemManager : scene) {
            director.getSystemManager(systemManager).updateSystems();
        }
        EndDrawing();
    }
    destroyRaylib();
}

void SceneManager::changeScene(Scene scene)
{
    _currentScene = scene;
    Registry::getInstance().clear();
}

void SceneManager::stop()
{
    _stop = true;
}
