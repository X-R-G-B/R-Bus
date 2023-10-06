/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ServerNitwork
*/

#pragma once

#include <any>
#include <boost/asio.hpp>

namespace Systems {
    void handleLifeUpdateMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
}