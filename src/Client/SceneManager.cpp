/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "ClientSystems.hpp"
#include "CustomTypes.hpp"
#include "Logger.hpp"
#include "Raylib.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"

namespace Scene {

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
        std::lock_guard<std::mutex> lock(director.mutex);

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

    static void updateSystemManagers(std::vector<SystemManagers> &scene)
    {
        auto &director = Systems::SystemManagersDirector::getInstance();

        for (auto &systemManager : scene) {
            std::unique_lock<std::mutex> lock(director.mutex);
            director.getSystemManager(static_cast<std::size_t>(systemManager)).updateSystems();
            lock.unlock();
        }
    }

    int SceneManager::run()
    {
        try {
            while (!_stop && !Raylib::windowShouldClose()) {
                Raylib::beginDrawing();
                Raylib::clearBackground(Raylib::DarkGray);
                auto scene = _scenes.at(static_cast<std::size_t>(_currentScene));
                updateSystemManagers(scene);
                Raylib::endDrawing();
            }
            destroyRaylib();
        } catch (std::exception &e) {
            Logger::fatal(e.what());
            return static_cast<int>(ReturnValue::RET_ERROR);
        }
        return static_cast<int>(ReturnValue::OK);
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

} // namespace Scene
