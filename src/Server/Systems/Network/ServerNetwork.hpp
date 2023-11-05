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
    void handleClientEnemyDeath(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
    void receiveNewBulletMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
    void receiveAbsolutePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
    void receivePlayerDeathMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
    void handleClientMissileDeath(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
} // namespace Systems
