/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** MainServerNetwork
*/

#include <string>
#include "Registry.hpp"
#include "MainNitwork.h"
#include "MainServerNetwork.hpp"

namespace Systems {
    void handleNewLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgNewLobby_s &newLobby = std::any_cast<struct msgNewLobby_s>(data);
        auto &registry                        = Registry::getInstance();
        auto &arrLobby                        = registry.getComponents<struct lobby_s>();
        auto lobbyIds                         = arrLobby.getExistingsId();
        auto isAlreadyPresent = std::find_if(lobbyIds.begin(), lobbyIds.end(), [&arrLobby, &newLobby](std::size_t id) {
            return std::string(arrLobby[id].lobbyInfos.ip) == std::string(newLobby.lobby.lobbyInfos.ip)
                && arrLobby[id].lobbyInfos.port == newLobby.lobby.lobbyInfos.port;
        });

        if (isAlreadyPresent != lobbyIds.end()) {
            return;
        }
        registry.addEntity();
        struct lobby_s lobbyToAdd = {newLobby.lobby};
        arrLobby.insertBack(lobbyToAdd);
        Logger::info("New lobby added: " + std::string(newLobby.lobby.lobbyInfos.ip) + ":"
            + std::to_string(newLobby.lobby.lobbyInfos.port));
    }
} // namespace Systems
