/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#pragma once

#include "UnloadRaylib.hpp"
#include "AudioSystems.hpp"
#include "DeathSystems.hpp"
#include "GraphicsCustomTypes.hpp"
#include "ParallaxSystems.hpp"
#include "Maths.hpp"
#include "Registry.hpp"
#include "SpriteSystems.hpp"
#include "TextSystems.hpp"
#include "SceneManager.hpp"
#include "IPlugin.hpp"

namespace Systems::GraphicsSystems {

    static void startLoopRaylib(std::size_t /*unused*/)
    {
        Raylib::Drawing::beginDrawing();
        Raylib::Drawing::clearBackground(Raylib::DarkGray);
    }

    constexpr int screenWidth  = 1920;
    constexpr int screenHeight = 1080;

    static void beforeLoopRaylib(std::size_t /*unused*/)
    {
        Raylib::Window::initWindow(screenWidth, screenHeight, "R-Bus");
        Raylib::Window::setWindowState(Raylib::ConfigFlags::WINDOW_RESIZABLE);
        Raylib::Frame::setTargetFPS(Raylib::Window::getMonitorRefreshRate(Raylib::Window::getCurrentMonitor()));
        Raylib::Audio::initAudioDevice();
    }

    static void endLoopRaylib(std::size_t /*unused*/)
    {
        Raylib::Drawing::endDrawing();
    }

    static void afterLoopRaylib(std::size_t /*unused*/)
    {
        Raylib::Audio::closeAudioDevice();
        Raylib::Window::closeWindow();
    }

    static void checkRaylibStop(std::size_t /*unused*/, std::size_t /*unused*/)
    {
        if (Raylib::Window::windowShouldClose()) {
            Scene::SceneManager::getInstance().stop();
        }
    }

    class GraphicsPlugin : public IPlugin
    {
        public:
            void initPlugin() override
            {
                Logger::fatal("initplugin");
                std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
                Registry::getInstance().addEventCallback(Events::REMOVE_ENTITY, UnloadRaylib::unloadRaylibComponents);
                Registry::getInstance().addEventCallback(Events::CLEAR, UnloadRaylib::unloadRaylibComponents);
                Registry::getInstance().addEventCallback(Events::START_LOOP, startLoopRaylib);
                Registry::getInstance().addEventCallback(Events::END_LOOP, endLoopRaylib);
                Registry::getInstance().addEventCallback(Events::BEFORE_LOOP, beforeLoopRaylib);
                Registry::getInstance().addEventCallback(Events::AFTER_LOOP, afterLoopRaylib);
            }
            std::vector<std::function<void(std::size_t, std::size_t)>> getSystems() override
            {
                Logger::fatal("getSystems graphics");
                std::vector<std::function<void(std::size_t, std::size_t)>> graphicsSystems = {
                    checkRaylibStop
                };
                std::vector<std::function<void(std::size_t, std::size_t)>> audioSystems  = AudioSystems::getAudioSystems();
                std::vector<std::function<void(std::size_t, std::size_t)>> spriteSystems = getSpriteSystems();
                std::vector<std::function<void(std::size_t, std::size_t)>> textSystems   = getTextSystems();
                std::vector<std::function<void(std::size_t, std::size_t)>> parallaxSystems =
                    ParallaxSystems::getParallaxSystems();

                graphicsSystems.insert(graphicsSystems.end(), audioSystems.begin(), audioSystems.end());
                graphicsSystems.insert(graphicsSystems.end(), spriteSystems.begin(), spriteSystems.end());
                graphicsSystems.insert(graphicsSystems.end(), textSystems.begin(), textSystems.end());
                graphicsSystems.insert(graphicsSystems.end(), parallaxSystems.begin(), parallaxSystems.end());
                return graphicsSystems;
            }
    };
} // namespace Systems
