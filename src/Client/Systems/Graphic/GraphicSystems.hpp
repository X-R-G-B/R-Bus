/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>

namespace Systems {
    namespace GraphicSystems {
        void rectRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        void spriteRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        void textRenderer(std::size_t /*unused*/, std::size_t /*unused*/);
        void soundEffectPlayer(std::size_t /*unused*/, std::size_t /*unused*/);
        void musicPlayer(std::size_t /*unused*/, std::size_t /*unused*/);
        void playSoundWithKey(std::size_t /*unused*/, std::size_t /*unused*/);
        void setEntityDeathFunction(std::size_t /*unused*/, std::size_t /*unused*/);
        std::vector<std::function<void(std::size_t, std::size_t)>>
        getGraphicsSystems();
    } // namespace GraphicSystems
} // namespace Systems
