#pragma once

#include <any>
#include <boost/asio.hpp>
#include <vector>

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void receiveEnemyDeath(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void handleStartWave(std::any &any, boost::asio::ip::udp::endpoint &);
    void receiveNewEnemy(std::any &any, boost::asio::ip::udp::endpoint &);
    void receiveNewPlayer(std::any &any, boost::asio::ip::udp::endpoint &);
    void sendPositionRelative(std::size_t /* unused */, std::size_t /* unused */);
    void receiveNewBullet(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void receiveRelativePosition(std::any &any, boost::asio::ip::udp::endpoint &);
    void receiveBroadcastAbsolutePosition(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void receivePlayerDeath(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void handleNewLobbyMsg(std::any &data, boost::asio::ip::udp::endpoint &endpoint);
    std::vector<std::function<void(std::size_t, std::size_t)>> getNetworkSystems();
} // namespace Systems
