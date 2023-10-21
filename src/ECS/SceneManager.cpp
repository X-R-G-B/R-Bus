/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager implementation
*/

#include "SceneManager.hpp"
#include "Logger.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "ECSSystems.hpp"
#ifdef GRAPHICS
    #include "Raylib.hpp"
    #include "GraphicsSystems.hpp"
#endif

namespace Scene {

    constexpr int screenWidth  = 1920;
    constexpr int screenHeight = 1080;

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    bool SceneManager::_init             = false;
    SceneManager SceneManager::_instance = SceneManager();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    static void initRaylib()
    {
#ifdef GRAPHICS
        Raylib::initWindow(screenWidth, screenHeight, "R-Bus");
        Raylib::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE);
        Raylib::setTargetFPS(Raylib::getMonitorRefreshRate(Raylib::getCurrentMonitor()));
        Raylib::initAudioDevice();
#endif
    }

    SceneManager &SceneManager::getInstance()
    {
        if (!_init) {
            _init = true;
            initRaylib();
        }
        return _instance;
    }

    SceneManager::SceneManager() : _currentScene(0), _stop(false)
    {
    }

    static void destroyRaylib()
    {
#ifdef GRAPHICS
        Raylib::closeAudioDevice();
        Raylib::closeWindow();
#endif
    }

    static void updateSystemManagers(std::vector<std::size_t> &scene)
    {
        auto &director = Systems::SystemManagersDirector::getInstance();

        for (auto &systemManager : scene) {
            std::unique_lock<std::mutex> lock(director.mutex);
            director.getSystemManager(systemManager).updateSystems();
            lock.unlock();
        }
    }

    int SceneManager::run()
    {
        try {
            while (!_stop
#ifdef GRAPHICS
                   && !Raylib::windowShouldClose()) {
                Raylib::beginDrawing();
                Raylib::clearBackground(Raylib::DarkGray);
#else
                   ) {
#endif
                auto scene = _scenes.at(static_cast<std::size_t>(_currentScene));
                updateSystemManagers(scene);
#ifdef GRAPHICS
                Raylib::endDrawing();
#endif
            destroyRaylib();
            }
        } catch (std::exception &e) {
            Logger::fatal(e.what());
            return static_cast<int>(ReturnValue::RET_ERROR);
        }
        return static_cast<int>(ReturnValue::OK);
    }

    void SceneManager::changeScene(std::size_t scene)
    {
        _currentScene = scene;
        Registry::getInstance().clear();
        Systems::SystemManagersDirector::getInstance().resetChanges();
    }

    std::size_t SceneManager::getCurrentScene() const
    {
        return _currentScene;
    }

    void SceneManager::stop()
    {
        _stop = true;
    }

    void SceneManager::setScenes(std::vector<std::vector<std::size_t>> scenes)
    {
        _scenes = scenes;
    }

    void SceneManager::addScene(std::vector<std::size_t> scene)
    {
        _scenes.push_back(scene);
    }

} // namespace Scene
