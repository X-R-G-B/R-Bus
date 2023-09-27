/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "ClientSystems.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "Raylib.hpp"

// to suppr
#include "CustomTypes.hpp"

constexpr int screenWidth  = 800;
constexpr int screenHeight = 600;

// to suppr
constexpr int playerData = 10;

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
bool SceneManager::_init             = false;
SceneManager SceneManager::_instance = SceneManager();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

static void initRaylib()
{
    Raylib::initWindow(screenWidth, screenHeight, "R-Bus");
    Raylib::setTargetFPS(Raylib::getMonitorRefreshRate(Raylib::getCurrentMonitor()));
    Raylib::initAudioDevice();
    Registry::getInstance().addEntity();
    Registry::getInstance().getComponents<Types::Position>().back() = {
        playerData,
        playerData};
    Registry::getInstance().getComponents<Types::RectangleShape>().back() = {
        playerData,
        playerData};
    Registry::getInstance().getComponents<Types::CollisionRect>().back() = {
        playerData,
        playerData};
    SparseArray<std::size_t> &playerId =
        Registry::getInstance().getCustomSparseArray<std::size_t>(
            CustomIndex::PLAYER);
    playerId.add();
    playerId.back() =
        std::optional<std::size_t>(Registry::getInstance().getEntitiesNb() - 1);
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

SceneManager::SceneManager() : _currentScene(Scene::MAIN_GAME), _stop(false)
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
        while (!_stop && !WindowShouldClose()) {
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
}

void SceneManager::stop()
{
    _stop = true;
}
