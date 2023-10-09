#pragma once

#include <any>
#include <boost/asio.hpp>

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
    void receiveEnemyDeath(std::any &any, boost::asio::ip::udp::endpoint &endpoint);
} // namespace Systems
