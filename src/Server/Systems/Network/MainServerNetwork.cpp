/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** MainServerNetwork
*/

#include "Nitwork.h"
#include "Registry.hpp"
#include "NitworkMainServer.hpp"
#include "MainServerNetwork.hpp"

namespace Systems {
    void handleListLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgRequestListLobby_s &msg = std::any_cast<struct msgRequestListLobby_s>(data);
        auto &registry = Registry::getInstance();
        auto &arrLobby = registry.getComponents<struct lobby_s>();
        auto lobbyIds = arrLobby.getExistingsId();

        if (msg.magick != MAGICK_LIST_LOBBY) {
            Logger::error("MAGICK_LIST_LOBBY is not the same");
            return;
        }
        std::vector<struct lobby_s> lobbies;
        for (auto &id : lobbyIds) {
            lobbies.push_back(arrLobby[id]);
            if (lobbies.size() == 5 || id == lobbyIds.back()) {
                Nitwork::NitworkMainServer::getInstance().sendListLobby(endpoint, lobbies);
                lobbies.clear();
            }
        }
    }
} // namespace Systems
