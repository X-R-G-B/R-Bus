/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AudioSystems
*/

#pragma once

#include "GraphicsCustomTypes.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "IPlugin.hpp"

namespace Systems {
    namespace GraphicsSystems {
        void soundEffectPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
            Registry::components<Raylib::Sound> arrSoundEffect =
                Registry::getInstance().getComponents<Raylib::Sound>();

            for (auto &soundEffect : arrSoundEffect) {
                if (soundEffect.NeedToPlay() && !soundEffect.isPlaying()) {
                    soundEffect.play();
                    soundEffect.setNeedToPlay(false);
                }
            }
        }
        void musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
            Registry::components<Raylib::Music> arrMusics =
                Registry::getInstance().getComponents<Raylib::Music>();

            for (auto &music : arrMusics) {
                if (music.NeedToPlay()) {
                    music.play();
                    music.setNeedToPlay(false);
                }
                if (music.isPlaying()) {
                    music.update();
                }
            }
        }

        class AudioSystems {
            public:
                static std::vector<std::function<void(std::size_t, std::size_t)>> getAudioSystems()
                {
                    return { soundEffectPlayer, musicPlayer };
                }
        };
    } // namespace GraphicsSystems
} // namespace Systems
