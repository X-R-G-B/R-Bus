/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** SelectLobby
*/

#include "SelectLobbySystems.hpp"
#include "NitworkClient.hpp"

namespace Systems {
    namespace SelectLobbySystems {
        void sendListLobby(std::size_t /*unused*/, std::size_t /*unused*/)
        {
            static bool first = true;

            if (first) {
                first = false;
                Nitwork::NitworkClient::getInstance().addListLobbyMsg();
            }
        }
        std::vector<std::function<void(std::size_t, std::size_t)>> getLobbySystems()
        {
            return {sendListLobby};
        }
    }
}