/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkServer
*/

#pragma once

#include "ANitwork.hpp"
#include "ServerNetwork.hpp"

namespace Nitwork {
    class NitworkServer : public ANitwork {
        public:
            ~NitworkServer() override              = default;
            NitworkServer(const NitworkServer &)   = delete;
            NitworkServer(const NitworkServer &&)  = delete;
            void operator=(const NitworkServer &)  = delete;
            void operator=(const NitworkServer &&) = delete;

            static NitworkServer &getInstance();

            bool startServer(
                int nbPlayer,
                const std::string &name,
                const std::string &ownerIp,
                int ownerPort,
                int port     = RANDOM_PORT,
                int threadNb = DEFAULT_THREAD_NB,
                int tick     = TICKS);

            struct lobby_s getServerInfos() const;

            /* Messages creation methods */
            void addInfoMsg();

            void addStarWaveMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t enemyId);

            void addLifeUpdateMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                n_id_t playerId,
                const struct health_s &life);

            void addEnemyDeathMessage(n_id_t enemyId);

            void addNewEnemyMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                const struct enemy_infos_s &enemyInfos);

            void addPlayerInitMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                const msgCreatePlayer_s &playerMsg);

            void broadcastNewBulletMsg(
                const struct msgNewBullet_s &msg,
                boost::asio::ip::udp::endpoint &senderEndpoint);

            void broadcastAbsolutePositionMsg(
                const struct position_absolute_s &pos,
                boost::asio::ip::udp::endpoint &senderEndpoint);

            void addPlayerDeathMsg(n_id_t id);

            void addNewPlayerMsg(
                boost::asio::ip::udp::endpoint &endpoint,
                const struct msgCreatePlayer_s &playerMsg);

            n_id_t getPlayerId(const boost::asio::ip::udp::endpoint &endpoint) const;

        private:
            NitworkServer() = default;

            std::unordered_map<boost::asio::ip::udp::endpoint, n_id_t> _playersIds;

            bool startNitworkConfig(int port, const std::string &ip) final;

            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

            void sendAlliesAlreadyPresent(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId);

            void sendNewAllie(
                n_id_t playerId,
                struct packetCreatePlayer_s packetMsgCreatePlayer,
                boost::asio::ip::udp::endpoint &endpoint,
                bool butNoOne = true);

            /* BEGIN handle messages methods */
            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleRelativePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
            /* END handle messages methods */

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkServer _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            lobby_s _serverInfos;

            // maps that will be used to handle the actions, in order to send or receive them
            std::map<enum n_actionType_t, std::pair<handleBodyT, actionHandler>> _actionsHandlers = {
                {INIT,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgInit_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleInitMsg(msg, endpoint);
                  }}},
                {READY,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgReady_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleReadyMsg(msg, endpoint);
                  }}},
                {POSITION_RELATIVE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPositionRelative_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleRelativePositionMsg(msg, endpoint);
                  }}},
                {LIFE_UPDATE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgLifeUpdate_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::handleLifeUpdateMsg(msg, endpoint);
                  }}},
                {ENEMY_DEATH,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgEnemyDeath_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::handleClientEnemyDeath(msg, endpoint);
                  }}},
                {NEW_BULLET,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgNewBullet_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receiveNewBulletMsg(msg, endpoint);
                  }}},
                {POSITION_ABSOLUTE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPositionAbsolute_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receiveAbsolutePositionMsg(msg, endpoint);
                  }}},
                {PLAYER_DEATH,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPlayerDeath_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receivePlayerDeathMsg(msg, endpoint);
                  }}},
            };
            std::map<enum n_actionType_t, actionSender> _actionToSendHandlers = {
                {
                 INIT, [this](Packet &packet) {
                        sendData<struct packetCreatePlayer_s>(packet);
                    }, },
                {LIFE_UPDATE,
                 [this](Packet &packet) {
                     sendData<struct packetLifeUpdate_s>(packet);
                 }},
                {START_WAVE,
                 [this](Packet &packet) {
                     sendData<struct packetMsgStartWave_s>(packet);
                 }},
                {ENEMY_DEATH,
                 [this](Packet &packet) {
                     sendData<struct packetEnemyDeath_s>(packet);
                 }},
                {NEW_ENEMY,
                 [this](Packet &packet) {
                     sendData<struct packetNewEnemy_s>(packet);
                 }},
                {NEW_BULLET,
                 [this](Packet &packet) {
                     sendData<struct packetNewBullet_s>(packet);
                 }},
                {NEW_PLAYER,
                 [this](Packet &packet) {
                     sendData<struct packetCreatePlayer_s>(packet);
                 }},
                {POSITION_RELATIVE_BROADCAST,
                 [this](Packet &packet) {
                     sendData<struct packetPositionRelativeBroadcast_s>(packet);
                 }},
                {POSITION_ABSOLUTE_BROADCAST,
                 [this](Packet &packet) {
                     sendData<struct packetPositionAbsoluteBroadcast_s>(packet);
                 }},
                {PLAYER_DEATH,
                 [this](Packet &packet) {
                     sendData<struct packetPlayerDeath_s>(packet);
                 }},
                {INFO_LOBBY, [this](Packet &packet) {
                     sendData<struct packetInfoLobby_s>(packet);
                 }}
            };
    };
} // namespace Nitwork
