/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** AudioSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicSystems {
        void soundEffectPlayer(std::size_t /*unused*/, std::size_t /*unused*/);
        void musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/);
        void playSoundWithKey(std::size_t, std::size_t);
        void initAudio(std::size_t, std::size_t);
        std::vector<std::function<void(std::size_t, std::size_t)>> getAudioSystems();
    } // namespace GraphicSystems
} // namespace Systems
