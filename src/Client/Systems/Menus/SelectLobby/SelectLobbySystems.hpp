/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#pragma once

#include <cstddef>
#include <functional>
#include <string>
#include <vector>
#include "B-luga-physics/ECSCustomTypes.hpp"
extern "C"
{
#include "MessageTypes.h"
}

namespace Systems::SelectLobbySystems {
    struct LobbyStatus {
            LobbyStatus(const std::string &ip = "", n_port_t port = -1);

            std::string ip;
            n_port_t port;
            static std::size_t pageNbr;
            static std::size_t pageMax;
    };

    void sendListLobby(std::size_t /*unused*/, std::size_t /*unused*/);
    std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems();
} // namespace Systems::SelectLobbySystems
