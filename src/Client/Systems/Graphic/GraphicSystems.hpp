/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** GraphicSystems
*/

#pragma once

#include <cstddef>
#include <vector>
#include <functional>

namespace Systems {
    namespace GraphicSystems {
        void rectRenderer(std::size_t /*unused*/);
        void spriteRenderer(std::size_t /*unused*/);
        void textRenderer(std::size_t /*unused*/);
        void soundEffectPlayer(std::size_t /*unused*/);
        void musicPlayer(std::size_t /*unused*/);
        const std::vector<std::function<void(std::size_t)>> getGraphicsSystems();
    } // namespace GraphicSystems
}
