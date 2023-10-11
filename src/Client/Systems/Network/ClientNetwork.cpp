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
        std::vector<std::size_t> ids                    = Registry::getInstance().getEntitiesByComponents(
            {typeid(struct health_s), typeid(Types::Player)});

        if (ids.empty()) {
            return;
        }
        struct health_s &life = arrHealth[ids[0]];
        if (life.hp != msg.life.hp) {
            life.hp = msg.life.hp;
        }
    }

    void receiveEnemyDeath(std::any &any, boost::asio::ip::udp::endpoint &)
    {
        const auto enemyDeath                      = std::any_cast<struct msgEnemyDeath_s>(any);
        Registry::components<Types::Enemy> enemies = Registry::getInstance().getComponents<Types::Enemy>();
        std::vector<std::size_t> ids               = enemies.getExistingsId();

        for (auto id : ids) {
            if (enemies[id].getConstId().id == enemyDeath.enemyId.id) {
                Registry::getInstance().removeEntity(id);
                return;
            }
        }
    }

    void sendPositionRelative(std::size_t /* unused */, std::size_t /* unused */)
    {
        constexpr std::size_t delay = 10;
        static auto clockId         = Registry::getInstance().getClock().create();
        static std::pair<std::size_t, Types::Position> positionPlayerCached;
        Registry &registry = Registry::getInstance();

        if (registry.getClock().elapsedMillisecondsSince(clockId) < delay) {
            return;
        }
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::Player)});
        auto arrPosition = registry.getComponents<Types::Position>();

        if (ids.empty()) {
            return;
        }
        if (positionPlayerCached.first != ids[0]) {
            positionPlayerCached.first  = ids[0];
            positionPlayerCached.second = arrPosition[ids[0]];
        }
        auto &posCached = positionPlayerCached.second;
        const auto &pos = arrPosition[positionPlayerCached.first];
        if (pos.x != posCached.x || pos.y != posCached.y) {
            registry.getClock().restart(clockId);
            struct position_relative_s msg = {
                .x = static_cast<char>(static_cast<int>(pos.x - posCached.x)),
                .y = static_cast<char>(static_cast<int>(pos.y - posCached.y)),
            };
            Nitwork::NitworkClient::getInstance().addPositionRelativeMsg(msg);
        }
    }

    void sendPositionAbsolute(std::size_t /* unused */, std::size_t /* unused */)
    {
        constexpr std::size_t delay = 1;
        static auto clockId         = Registry::getInstance().getClock().create();
        Registry &registry          = Registry::getInstance();

        if (registry.getClock().elapsedSecondsSince(clockId) < delay) {
            return;
        }
        auto ids = registry.getEntitiesByComponents({typeid(Types::Position), typeid(Types::Player)});
        auto arrPosition = registry.getComponents<Types::Position>();

        if (ids.empty()) {
            return;
        }
        struct position_absolute_s msg = {
            .x = static_cast<int>(arrPosition[ids[0]].x),
            .y = static_cast<int>(arrPosition[ids[0]].y),
        };
        Nitwork::NitworkClient::getInstance().addPositionAbsoluteMsg(msg);
    }

    std::vector<std::function<void(std::size_t, std::size_t)>> getNetworkSystems()
    {
        return {sendPositionRelative, sendPositionAbsolute};
    }
} // namespace Systems
