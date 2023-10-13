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
#include "Systems.hpp"

namespace Systems {
    void handleLifeUpdateMsg(const std::any &any, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
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
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        const struct msgEnemyDeath_s &msgEnemyDeath = std::any_cast<struct msgEnemyDeath_s>(msg);
        auto &registry                              = Registry::getInstance();

        if (msgEnemyDeath.magick != MAGICK_ENEMY_DEATH) {
            Logger::error("Error: magick is not CLIENT_ENEMY_DEATH");
            return;
        }
        auto &arrEnemies = registry.getComponents<Types::Enemy>();
        auto arrHealth   = registry.getComponents<struct health_s>();
        auto arrPos      = registry.getComponents<Types::Position>();
        auto it = std::find_if(arrEnemies.begin(), arrEnemies.end(), [&msgEnemyDeath](auto &enemy) {
            return enemy.getConstId().id == msgEnemyDeath.enemyId.id;
        });
        if (it == arrEnemies.end()) {
            return;
        }
        auto index = std::distance(arrEnemies.begin(), it);
        if (!arrEnemies.exist(index) || !arrHealth.exist(index) || !arrPos.exist(index)) {
            return;
        }
        Nitwork::NitworkServer::getInstance().addNewEnemyMessage(
            endpoint,
            {
                .id   = arrEnemies[index].getConstId(),
                .life = arrHealth[index],
                .pos =
                    {static_cast<char>(static_cast<int>(arrPos[index].x)),
                          static_cast<char>(static_cast<int>(arrPos[index].y))},
                .type = arrEnemies[index].getType(),
        });
    }

    void receiveNewBulletMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);

        const struct msgNewBullet_s &msgNewBullet = std::any_cast<struct msgNewBullet_s>(msg);

        struct Types::Position position = {
            static_cast<float>(msgNewBullet.pos.x),
            static_cast<float>(msgNewBullet.pos.y),
        };
        struct Types::Missiles missileType = {static_cast<missileTypes_e>(msgNewBullet.missileType)};
        Systems::createMissile(position, missileType);
//         send bullet to clients but not the sender
        Nitwork::NitworkServer::getInstance().broadcastNewBulletMsg(msgNewBullet, endpoint);
    }
} // namespace Systems
