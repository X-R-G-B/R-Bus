/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ServerNitwork
*/

#include "ServerNetwork.hpp"
#include "ECSCustomTypes.hpp"
#include "NitworkServer.hpp"
#include "Registry.hpp"

namespace Systems {
    void handleLifeUpdateMsg(const std::any &any, boost::asio::ip::udp::endpoint &endpoint)
    {
        auto msg              = std::any_cast<struct msgLifeUpdate_s>(any);
        Registry &registry    = Registry::getInstance();
        auto &arrHealth       = registry.getComponents<struct health_s>();
        auto &arrOtherPlayers = registry.getComponents<Types::OtherPlayer>();
        std::vector<std::size_t> ids =
            registry.getEntitiesByComponents({typeid(struct health_s), typeid(Types::OtherPlayer)});

        for (auto &id : ids) {
            auto &life        = arrHealth[id];
            auto &otherPlayer = arrOtherPlayers[id];
            if (otherPlayer.constId == msg.playerId && life.hp != msg.life.hp) {
                Nitwork::NitworkServer::getInstance().addLifeUpdateMessage(
                    endpoint,
                    msg.playerId,
                    msg.life);
                return;
            }
        }
    }

    void handleClientEnemyDeath(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
    {
        const struct msgClientEnemyDeath_s &msgClientEnemyDeath = std::any_cast<struct msgClientEnemyDeath_s>(msg);
        auto &registry = Registry::getInstance();

        if (msgClientEnemyDeath.magick != MAGICK_CLIENT_ENEMY_DEATH) {
            Logger::error("Error: magick is not CLIENT_ENEMY_DEATH");
            return;
        }
        auto &arrEnemies = registry.getComponents<Types::Enemy>();
        auto arrHealth = registry.getComponents<struct health_s>();
        auto arrPos = registry.getComponents<Types::Position>();
        auto it = std::find_if(arrEnemies.begin(), arrEnemies.end(), [&msgClientEnemyDeath](auto &enemy) {
            return enemy.constId.value == msgClientEnemyDeath.enemyId.value;
        });
        if (it == arrEnemies.end()) {
            return;
        }
        auto index = std::distance(arrEnemies.begin(), it);
        if (!arrEnemies.exist(index) || !arrHealth.exist(index) || !arrPos.exist(index)) {
            return;
        }
        Nitwork::NitworkServer::getInstance().addNewEnemyMessage(endpoint,
        {
            .id = arrEnemies[index].constId,
            .life = arrHealth[index],
            .pos = {static_cast<char>(static_cast<int>(arrPos[index].x)), static_cast<char>(static_cast<int>(arrPos[index].y))},
            .type = arrEnemies[index].type
        });
    }
} // namespace Systems
