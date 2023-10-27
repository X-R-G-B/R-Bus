/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#pragma once

#include <cstddef>
#include <functional>
#include <vector>
#include "ECSCustomTypes.hpp"

namespace Systems {
    namespace SelectLobbySystems {
        void sendListLobby(std::size_t /*unused*/, std::size_t /*unused*/);
        std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems();
    }
}
