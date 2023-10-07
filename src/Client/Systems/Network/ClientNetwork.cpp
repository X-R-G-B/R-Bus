#include "ClientNetwork.hpp"
#include <algorithm>
#include "ECSCustomTypes.hpp"
#include "NitworkClient.hpp"
#include "Registry.hpp"

namespace Systems {
    void receiveLifeUpdate(std::any &any, boost::asio::ip::udp::endpoint & /* unused */)
    {
        auto msg                                        = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry                              = Registry::getInstance();
        Registry::components<struct health_s> arrHealth = registry.getComponents<struct health_s>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(struct health_s), typeid(Types::Player)});

        for (auto id : ids) {
            struct health_s &life = arrHealth[id];
            if (life.hp != msg.life.hp) {
                life.hp = msg.life.hp;
            }
        }
    }

    void sendPositionRelative(std::size_t /* unused */, std::size_t /* unused */)
    {
        constexpr std::size_t delay = 10;
        static auto clockId = Registry::getInstance().getClock().create();
        static std::map<std::size_t, Types::Position> positionsPlayerCached;
        Registry &registry = Registry::getInstance();

        if (registry.getClock().elapsedMillisecondsSince(clockId) < delay) {
            return;
        }
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::Player)});
        auto arrPosition = registry.getComponents<Types::Position>();

        // send relative position
        for (auto id : ids) {
            if (!positionsPlayerCached.contains(id)) {
                positionsPlayerCached[id] = arrPosition[id];
            }
            auto &posCached = positionsPlayerCached[id];
            const auto &pos = arrPosition[id];
            if (pos.x != posCached.x || pos.y != posCached.y) {
                registry.getClock().restart(clockId);
                struct position_relative_s msg = {
                    .x = static_cast<char>(static_cast<int>(pos.x - posCached.x)),
                    .y = static_cast<char>(static_cast<int>(pos.y - posCached.y)),
                };
                Nitwork::NitworkClient::getInstance().addPositionRelativeMsg(msg);
            }
        }
        // remove unused id cached
        auto cursor = positionsPlayerCached.begin();
        while (cursor != positionsPlayerCached.end()) {
            if (std::find(ids.begin(), ids.end(), cursor->first) == ids.end()) {
                cursor = positionsPlayerCached.erase(cursor);
            } else {
                ++cursor;
            }
        }
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getNetworkSystems()
    {
        return {sendPositionRelative};
    }
} // namespace Systems
