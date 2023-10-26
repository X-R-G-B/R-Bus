/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** MainServerNetwork
*/

#include "MainServerNetwork.hpp"
#include "Nitwork.h"
#include "NitworkMainServer.hpp"
#include "Registry.hpp"

constexpr int LIST_LOBBY_SIZE = 5;

namespace Systems {
    void handleListLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint)
    {
        const struct msgRequestListLobby_s &msg = std::any_cast<struct msgRequestListLobby_s>(data);

        if (msg.magick != MAGICK_LIST_LOBBY) {
            Logger::error("MAGICK_LIST_LOBBY is not the same");
            return;
        }
        const auto &arrLobby = Nitwork::NitworkMainServer::getInstance().getLobbies();
        std::vector<struct lobby_s> lobbies;
        for (const auto &lobby : arrLobby) {
            lobbies.push_back(lobby);
            if (lobbies.size() == LIST_LOBBY_SIZE || &lobby == &arrLobby.back()) {
                Nitwork::NitworkMainServer::getInstance().sendListLobby(endpoint, lobbies);
                lobbies.clear();
            }
        }
    }

    void handleCreateLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &/* unused */)
    {
        const struct msgCreateLobby_s &msg = std::any_cast<struct msgCreateLobby_s>(data);

        if (msg.magick != MAGICK_CREATE_LOBBY) {
            Logger::error("MAGICK_CREATE_LOBBY is not the same");
            return;
        }
        Nitwork::NitworkMainServer::getInstance().createLobby(msg.maxNbPlayer, msg.name);
    }

    void handleInfoLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &/* unused */)
    {
        const struct msgInfoLobby_s &msg = std::any_cast<struct msgInfoLobby_s>(data);

        if (msg.magick != MAGICK_INFO_LOBBY) {
            Logger::error("MAGICK_HELLO_LOBBY is not the same");
            return;
        }
        Logger::info("Lobby " + std::string(msg.lobby.name) + " is ready");
        Nitwork::NitworkMainServer::getInstance().addLobby(msg.lobby);
    }
} // namespace Systems
