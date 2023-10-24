/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** MainServerNetwork
*/

#pragma once

#include <any>
#include <boost/asio.hpp>

namespace Systems {
    void handleListLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint);
} // namespace Systems
