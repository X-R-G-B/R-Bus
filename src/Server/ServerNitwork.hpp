/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ServerNitwork
*/

#pragma one

#include <any>
#include <boost/asio.hpp>

namespace Systems {
    void handleLifeUpdate(const std::any & msg, boost::asio::ip::udp::endpoint &endpoint);
}