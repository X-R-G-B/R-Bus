#pragma once

#include <any>
#include <boost/asio.hpp>
#include <vector>

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint &endpoint);

    void sendPositionRelative(std::size_t /* unused */, std::size_t /* unused */);

    std::vector<std::function<void(std::size_t, std::size_t)>> getNetworkSystems();
} // namespace Systems
