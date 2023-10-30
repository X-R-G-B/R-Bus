/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SceneManager
*/

#pragma once

#include "SceneManager.hpp"
#include "Logger.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "ECSSystems.hpp"

namespace Scene {

    enum class ReturnValue { OK = 0, RET_ERROR = 84 };

    class SceneManager {
        public:
            static SceneManager &getInstance()
            {
                static SceneManager instance;
                return instance;
            }

            int run()
            {
                try {
                    Registry::getInstance().callback(Events::BEFORE_LOOP);
                    while (!_stop) {
                        Registry::getInstance().callback(Events::START_LOOP);
                        auto scene = _scenes.at(static_cast<std::size_t>(_currentScene));
                        updateSystemManagers(scene);
                        Registry::getInstance().removeEntitiesToRemove();
                        Registry::getInstance().callback(Events::END_LOOP);
                    }
                    Registry::getInstance().callback(Events::AFTER_LOOP);
                } catch (std::exception &e) {
                    Logger::fatal(e.what());
                    return static_cast<int>(ReturnValue::RET_ERROR);
                }
                return static_cast<int>(ReturnValue::OK);
            }

            void changeScene(std::size_t scene)
            {
                _currentScene = scene;
                Registry::getInstance().clear();
                Systems::SystemManagersDirector::getInstance().resetChanges();
            }

            std::size_t getCurrentScene() const
            {
                return _currentScene;
            }

            void stop()
            {
                _stop = true;
            }

            void setScenes(std::vector<std::vector<std::size_t>> scenes)
            {
                _scenes = scenes;
            }

            void addScene(std::vector<std::size_t> scene)
            {
                _scenes.push_back(scene);
            }

        private:
            SceneManager() : _currentScene(0), _stop(false) {};

            void updateSystemManagers(std::vector<std::size_t> &scene)
            {
                auto &director = Systems::SystemManagersDirector::getInstance();

                for (auto &systemManager : scene) {
                    std::unique_lock<std::mutex> lock(director.mutex);
                    try {
                        director.getSystemManager(systemManager).updateSystems();
                    } catch (std::exception &e) {
                        Logger::fatal(e.what());
                    }
                    lock.unlock();
                }
            }

            std::size_t _currentScene;
            bool _stop;
            std::vector<std::vector<std::size_t>> _scenes;
    };
} // namespace Scene
