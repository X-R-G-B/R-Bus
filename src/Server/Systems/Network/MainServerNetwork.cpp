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
    void handleRequestListLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint)
    {
        const struct msgRequestListLobby_s &msg = std::any_cast<struct msgRequestListLobby_s>(data);

        if (msg.magick != MAGICK_REQUEST_LIST_LOBBY) {
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

    void handleCreateLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint & /* unused */)
    {
        const struct msgCreateLobby_s &msg = std::any_cast<struct msgCreateLobby_s>(data);

        if (msg.magick != MAGICK_CREATE_LOBBY) {
            Logger::error("MAGICK_CREATE_LOBBY is not the same");
            return;
        }
        for (const auto &lobby : Nitwork::NitworkMainServer::getInstance().getLobbies()) {
            if (std::strcmp(lobby.name, msg.name) == 0) {
                Logger::error("Lobby " + std::string(msg.name) + " already exist");
                return;
            }
        }
        Nitwork::NitworkMainServer::getInstance().createLobby(msg.maxNbPlayer, msg.name, msg.gameType);
    }

    void handleInfoLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &lobbyEndpoint)
    {
        const struct msgInfoLobby_s &msg = std::any_cast<struct msgInfoLobby_s>(data);
        struct lobby_s lobby             = {};

        if (msg.magick != MAGICK_INFO_LOBBY) {
            Logger::error("MAGICK_HELLO_LOBBY is not the same");
            return;
        }
        Logger::info(
            "Lobby " + std::string(msg.name) + " is ready (" + lobbyEndpoint.address().to_string() + ":"
            + std::to_string(lobbyEndpoint.port()) + ")");
        std::strcpy(lobby.name, msg.name);
        lobby.maxNbPlayer = msg.maxNbPlayer;
        lobby.gameType    = msg.gameType;
        std::strcpy(lobby.lobbyInfos.ip, lobbyEndpoint.address().to_string().c_str());
        lobby.lobbyInfos.port = lobbyEndpoint.port();
        lobby.ownerInfos      = msg.ownerInfos;
        Nitwork::NitworkMainServer::getInstance().addLobby(lobby);
    }
} // namespace Systems
