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

            bool start(
                int port,
                int threadNb          = DEFAULT_THREAD_NB,
                int tick              = TICKS_PER_SECOND,
                const std::string &ip = "") final;

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

        private:
            NitworkServer() = default;

            bool startNitworkConfig(int port, const std::string &ip) final;

            void sendToAllClients(const Packet &packet);

            void sendToAllClientsButNotOne(const Packet &packet, boost::asio::ip::udp::endpoint &endpoint);

            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            [[nodiscard]] const std::map<enum n_actionType_t, actionHandler> &
            getActionToSendHandlers() const final;

            bool isClientAlreadyConnected(boost::asio::ip::udp::endpoint &endpoint) const;

            /* BEGIN handle messages methods */
            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
            /* END handle messages methods */
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkServer _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
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
                  }}}
            };
            std::map<enum n_actionType_t, actionHandler> _actionToSendHandlers = {
                {
                 INIT, [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                        sendData<struct packetMsgPlayerInit_s>(any, endpoint);
                    }, },
                {LIFE_UPDATE,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetLifeUpdate_s>(any, endpoint);
                 }},
                {START_WAVE,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetMsgStartWave_s>(any, endpoint);
                 }},
                {LIFE_UPDATE,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetLifeUpdate_s>(any, endpoint);
                 }},
                {ENEMY_DEATH,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetEnemyDeath_s>(any, endpoint);
                 }},
                {NEW_ENEMY,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetNewEnemy_s>(any, endpoint);
                 }},
                {NEW_BULLET,
                 [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                     sendData<struct packetNewBullet_s>(any, endpoint);
                 }},
            };
    };
} // namespace Nitwork
