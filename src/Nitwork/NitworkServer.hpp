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
                int port,
                int nbPlayer,
                int threadNb          = DEFAULT_THREAD_NB,
                int tick              = TICKS_PER_SECOND);

            /* Messages creation methods */
            void addStarWaveMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t enemyId);

            void addLifeUpdateMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                n_id_t playerId,
                const struct health_s &life);

            void addEnemyDeathMessage(n_id_t enemyId);

            void addNewEnemyMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                const struct enemy_infos_s &enemyInfos);

            void addPlayerInitMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId);

            void broadcastNewBulletMsg(
                const struct msgNewBullet_s &msg,
                boost::asio::ip::udp::endpoint &senderEndpoint);

            void broadcastAbsolutePositionMsg(
                const struct position_absolute_s &pos,
                boost::asio::ip::udp::endpoint &senderEndpoint);

            n_id_t getPlayerId(const boost::asio::ip::udp::endpoint &endpoint) const;

        private:
            NitworkServer() = default;

            std::unordered_map<boost::asio::ip::udp::endpoint, n_id_t> _playersIds;

            bool startNitworkConfig(int port, const std::string &ip) final;

            void sendToAllClients(const Packet &packet);

            void sendToAllClientsButNotOne(const Packet &packet, boost::asio::ip::udp::endpoint &endpoint);

            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

            bool isClientAlreadyConnected(boost::asio::ip::udp::endpoint &endpoint) const;

            /* BEGIN handle messages methods */
            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleRelativePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
            /* END handle messages methods */

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkServer _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            unsigned int _maxNbPlayer = 0; // max number of players
            std::list<boost::asio::ip::udp::endpoint>
                _endpoints; // A vector of endpoints which will be used to send the actions to the clients
                            // and identify them

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
            };
            std::map<enum n_actionType_t, actionSender> _actionToSendHandlers = {
                {
                 INIT, [this](Packet &packet) {
                        sendData<struct packetMsgPlayerInit_s>(packet);
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
                {POSITION_ABSOLUTE_BROADCAST,
                 [this](Packet &packet) {
                     sendData<struct packetPositionAbsolute_s>(packet);
                 }},
            };
    };
} // namespace Nitwork
