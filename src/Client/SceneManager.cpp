/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "raylib.h"
#include "ClientSystems.hpp"
#include "SceneManager.hpp"
#include "SystemManagersDirector.hpp"

#include <iostream>
#include "Registry.hpp"
#include "CustomTypes.hpp"

constexpr int screenWidth = 1920;
constexpr int screenHeight = 1080;
constexpr int fps = 60;

std::optional<SceneManager> SceneManager::_instance = std::nullopt;

SceneManager &SceneManager::getInstance()
{
    if (_instance == std::nullopt) {
        std::cout << "SCENE CONTRUCTION" << std::endl;
        const std::optional<SceneManager> a = SceneManager();
        _instance = a;
    }
    return _instance.value();
}

static void initRaylib()
{
    InitWindow(
        screenWidth,
        screenHeight,
        "raylib [textures] examples - texture source and destination "
        "rectangles");
    SetTargetFPS(fps);
    InitAudioDevice();
    Registry &registry = Registry::getInstance();
    registry.addEntity();
    Registry::components<Types::CollisionRect> res = registry.getComponents<Types::CollisionRect>();
    res.back() = {10, 20};
    registry.getComponents<Types::Player>().back() = Types::Player(true);
    registry.getComponents<Types::Position>().back()      = {0, 0};
    registry.getComponents<Types::Sprite>().back()        = {
               "./assets/R-TypeSheet/r-typesheet18.gif",
               10,
               20};
    registry.getComponents<Types::Rect>().back()   = {2.0F, 5.0F, 30.5F, 25.2F};
    registry.addEntity();
    registry.getComponents<Types::Position>().back()       = {0, 0};
    registry.getComponents<Types::RectangleShape>().back() = {10, 10};
    registry.addEntity();
    registry.getComponents<Types::Position>().back() = {40, 40};
    registry.getComponents<Types::Text>().back()     = {
            "Player",
            BLACK,
            LoadFont("assets/Fonts/soliden/SolidenTrial-Black.otf"),
            5.5};
    registry.addEntity();
    registry.getComponents<Types::SoundEffect>().back() =
        Types::SoundEffect("assets/Audio/Sounds/yes.ogg");
    registry.addEntity();
    registry.getComponents<Types::MusicStream>().back() =
        Types::MusicStream("assets/Audio/Musics/Title.mp3");
    if (registry.getComponents<Types::MusicStream>().back().has_value()) {
        registry.getComponents<Types::MusicStream>().back().value().needToPlay = true;
    }
}

static void destroyRaylib()
{
    CloseAudioDevice();
    CloseWindow();
}

SceneManager::SceneManager() : _currentScene(Scene::MAIN_GAME), _stop(false)
{
    std::cout << "CONSTRUCTEUR" << std::endl;
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
    std::cout << "changeScene ";
    std::cout << static_cast<int>(scene) << " " << static_cast<int>(_currentScene) << std::endl;
    _currentScene = scene;
    Registry::getInstance().clear();
    Registry &registry = Registry::getInstance();
    registry.addEntity();
    registry.getComponents<Types::Position>().back()       = {0, 0};
    registry.getComponents<Types::RectangleShape>().back() = {10, 10};
    registry.getComponents<Types::Player>().back() = Types::Player(true);
}

void SceneManager::stop()
{
    _stop = true;
}
