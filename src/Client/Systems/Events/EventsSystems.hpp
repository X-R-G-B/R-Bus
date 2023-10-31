/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** EventsSystems
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>
#include "B-luga-physics/ECSCustomTypes.hpp"

enum SceneType {
    MENU,
    MAIN_GAME
};

namespace Systems {
    namespace EventsSystems {
        void playerMovement(std::size_t /*unused*/, std::size_t /*unused*/);
        void changeScene(std::size_t /*unused*/, std::size_t /*unused*/);
        void shootBullet(std::size_t /*unused*/, std::size_t /*unused*/);
        void playSoundWithKey(std::size_t /*unused*/, std::size_t /*unused*/);
        std::vector<std::function<void(std::size_t, std::size_t)>> getEventsSystems();
    } // namespace EventsSystems
} // namespace Systems
