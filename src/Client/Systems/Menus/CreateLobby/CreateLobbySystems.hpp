/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** CreateLobbySystems
*/

#pragma once

#include <functional>
#include <vector>

namespace Systems {
    namespace CreateLobby {
        std::vector<std::function<void(std::size_t /*unused*/, std::size_t /*unused*/)>> getCreateLobbySystems();
    } // namespace CreateLobby
} // namespace Systems
