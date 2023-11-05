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

            /**
             * @brief Get the instance of the NitworkServer
             * @return the instance of the NitworkServer
             */
            static NitworkServer &getInstance();

            /**
             * @brief Start the server
             * @param nbPlayer the number of player that will be in the game
             * @param gameType the type of the game
             * @param name the name of the lobby
             * @param ownerIp The ip of the owner of the lobby
             * @param ownerPort The port of the owner of the lobby
             * @param port the port of the server
             * @param threadNb the number of thread that will be used by the server
             * @param tick the number of tick per second
             * @return true if the server has been started, false otherwise
             */
            bool startServer(
                int nbPlayer,
                enum gameType_e gameType,
                const std::string &name,
                const std::string &ownerIp,
                int ownerPort,
                int port     = RANDOM_PORT,
                int threadNb = DEFAULT_THREAD_NB,
                int tick     = TICKS);

            /**
             * @brief Get the server infos
             * @return the server infos
             */
            struct lobby_s getServerInfos() const;

            /* Messages creation methods */
            /**
             * @brief Add a msg packet that contain the new player msg to the clients
             * @param waveId The id of the wave that will be started
             */
            void addStarWaveMessage(n_id_t waveId);

            /**
             * @brief Add a msg that contain the lobby infos to the main server
             */
            void addInfoLobbyMsg();

            /**
             * @brief Add a msg that contain the lobby infos to the main server
             * @param endpoint The endpoint of the client that will receive the msg
             * @param canConnect A boolean that say if the client can connect to the lobby or not
             */
            void addConnectLobbyRespMsg(boost::asio::ip::udp::endpoint &endpoint, bool canConnect);

            /**
             * @brief Add a msg packet that contain the life update msg to the clients
             * @param endpoint The endpoint of the client that will receive the life update
             * @param playerId The id of the player that will receive the life update
             * @param life The life of the player
             */
            void addLifeUpdateMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                n_id_t playerId,
                const struct health_s &life);

            /**
             * @brief Add a msg packet that contain the enemy death msg to the clients
             * @param enemyId The id of the enemy that died
             */
            void addEnemyDeathMessage(n_id_t enemyId);

            /**
             * @brief Add a msg packet that contain the new enemy msg to the clients
             * @param endpoint The endpoint of the client that will receive the new enemy
             * @param enemyInfos The infos of the enemy that will be created
             */
            void addNewEnemyMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                const struct enemy_infos_s &enemyInfos);

            /**
             * @brief Add a msg packet that contain the new bullet msg to the clients
             * @param endpoint The endpoint of the client that sent the init msg
             * (in order to not send himself the msg)
             * @param playerMsg The infos of the bullet that will be created
             */
            void addPlayerInitMessage(
                boost::asio::ip::udp::endpoint &endpoint,
                const msgCreatePlayer_s &playerMsg);

            /**
             * @brief Add a msg packet that contain the new missile msg to the clients
             * @param msg The infos of the missile that will be created
             */
            void broadcastNewBulletMsg(const struct msgNewBullet_s &msg);

            /**
             * @brief Add a msg packet that contain the new bullet msg to the clients
             * @param pos The infos of the bullet that will be created
             * @param senderEndpoint The endpoint of the client that sent the msg of hie absolute position
             */
            void broadcastAbsolutePositionMsg(
                const struct position_absolute_s &pos,
                boost::asio::ip::udp::endpoint &senderEndpoint);

            /**
             * @brief Add a msg packet that contain the new bullet msg to the clients
             * @param id The id of the player that died
             */
            void addPlayerDeathMsg(n_id_t id);
            void addPlayerDeathMsg(n_id_t id, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Add a msg packet that contain the new bullet msg to the clients
             * @param endpoint The endpoint of the client that will receive the new player
             * @param playerMsg The infos of the player that will be created
             */
            void addNewPlayerMsg(
                boost::asio::ip::udp::endpoint &endpoint,
                const struct msgCreatePlayer_s &playerMsg);

            /**
             * @brief Add a msg packet that contain the new missile msg to the clients
             * @param id The id of the player that died
             */
            void addMissileDeathMsg(n_id_t id);

            /**
             * @brief Add a msg packet that contain the new end game msg to the clients
             */
            void addEndGameMsg();

            n_id_t getPlayerId(const boost::asio::ip::udp::endpoint &endpoint) const;

        private:
            /**
             * @brief Construct a new NitworkServer object
             */
            NitworkServer() = default;

            /**
             * @brief A map that contain the endpoint of the clients and their id
             * in order to retreive the id of a client with his endpoint
             */
            std::unordered_map<boost::asio::ip::udp::endpoint, n_id_t> _playersIds;

            /**
             * @brief A map that contain the endpoint of the clients and their infos
             * @param port The port of the server
             * @param ip The ip of the server
             * @return true if the server has been started, false otherwise
             */
            bool startNitworkConfig(int port, const std::string &ip) final;

            /**
             * @brief A map that contain the endpoint of the clients and their infos
             * @param header The header of the packet
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            /**
             * @brief A map that contain the endpoint of the clients and their infos
             * @return the infos of the server
             */
            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

            /**
             * @brief A map that contain the endpoint of the clients and their infos
             * @param endpoint The endpoint of the client that sent the packet
             * @param playerId The id of the player that sent the packet
             */
            void sendAlliesAlreadyPresent(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId);

            /**
             * @brief A map that contain the endpoint of the clients and their infos
             * @param playerId The id of the player that sent the packet
             * @param packetMsgCreatePlayer The infos of the player that will be created
             * @param endpoint The endpoint of the client that sent the packet
             * @param butNoOne A boolean that say if the packet must be sent to everyone or not
             */
            void sendNewAllie(
                n_id_t playerId,
                struct packetCreatePlayer_s packetMsgCreatePlayer,
                boost::asio::ip::udp::endpoint &endpoint,
                bool butNoOne = true);

            /* BEGIN handle messages methods */
            /**
             * @brief Handle the connect main server msg
             * send a response to the client if he can or not connect to the lobby
             * (if the server is full or not)
             * @param msg The msg that will be handled
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleConnectLobbyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Handle the init msg
             * @param msg The msg that will be handled
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Handle the ready msg
             * @param msg The msg that will be handled
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Handle the relative position msg
             * @param msg The msg that will be handled
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleRelativePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Handle the disconnect msg
             * @param msg The msg that will be handled
             * @param endpoint The endpoint of the client that sent the packet
             */
            void handleDisconnectMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
            /* END handle messages methods */

            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            /**
             * @brief The infos of the server
             */
            static NitworkServer _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            /**
             * @brief The infos of the server
             */
            lobby_s _serverInfos;

            std::unordered_map<boost::asio::ip::udp::endpoint /* endpoint */, bool /* isReady */>
                _playersReady;

            /**
             * @brief A boolean that say if the game has started or not
             */
            bool _isGameStarted = false;

            // maps
            /**
             * @brief a map that will be used to handle the actions, in order to receive them
             */
            std::map<enum n_actionType_t, std::pair<handleBodyT, actionHandler>> _actionsHandlers = {
                {NITWORK_CONNECT_LOBBY,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgConnectLobby_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleConnectLobbyMsg(msg, endpoint);
                  }}},
                {NITWORK_INIT,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgInit_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleInitMsg(msg, endpoint);
                  }}},
                {NITWORK_READY,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgReady_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleReadyMsg(msg, endpoint);
                  }}},
                {NITWORK_POSITION_RELATIVE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPositionRelative_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleRelativePositionMsg(msg, endpoint);
                  }}},
                {NITWORK_LIFE_UPDATE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgLifeUpdate_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::handleLifeUpdateMsg(msg, endpoint);
                  }}},
                {NITWORK_ENEMY_DEATH,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgEnemyDeath_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::handleClientEnemyDeath(msg, endpoint);
                  }}},
                {NITWORK_NEW_MISSILE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgNewBullet_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receiveNewBulletMsg(msg, endpoint);
                  }}},
                {NITWORK_POSITION_ABSOLUTE,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPositionAbsolute_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receiveAbsolutePositionMsg(msg, endpoint);
                  }}},
                {NITWORK_PLAYER_DEATH,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgPlayerDeath_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::receivePlayerDeathMsg(msg, endpoint);
                  }}},
                {NITWORK_MISSILE_DEATH,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgMissileDeath_s>(actionHandler, header);
                  },
                  [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      Systems::handleClientMissileDeath(msg, endpoint);
                  }}},
                {NITWORK_DISCONNECT_LOBBY,
                 {[this](actionHandler &actionHandler, const struct header_s &header) {
                      handleBody<struct msgDisconnectLobby_s>(actionHandler, header);
                  },
                  [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                      handleDisconnectMsg(msg, endpoint);
                  }}}
            };
            /**
             * @brief a map that will be used to handle the actions, in order to send them
             */
            std::map<enum n_actionType_t, actionSender> _actionToSendHandlers = {
                {NITWORK_CONNECT_LOBBY_RESP,
                 [this](Packet &packet) {
                     sendData<struct packetConnectLobbyResp_s>(packet);
                 }                            },
                {NITWORK_INIT,
                 [this](Packet &packet) {
                     sendData<struct packetCreatePlayer_s>(packet);
                 }                            },
                {NITWORK_LIFE_UPDATE,
                 [this](Packet &packet) {
                     sendData<struct packetLifeUpdate_s>(packet);
                 }                            },
                {NITWORK_START_WAVE,
                 [this](Packet &packet) {
                     sendData<struct packetMsgStartWave_s>(packet);
                 }                            },
                {NITWORK_ENEMY_DEATH,
                 [this](Packet &packet) {
                     sendData<struct packetEnemyDeath_s>(packet);
                 }                            },
                {NITWORK_NEW_ENEMY,
                 [this](Packet &packet) {
                     sendData<struct packetNewEnemy_s>(packet);
                 }                            },
                {NITWORK_NEW_MISSILE,
                 [this](Packet &packet) {
                     sendData<struct packetNewBullet_s>(packet);
                 }                            },
                {NITWORK_NEW_PLAYER,
                 [this](Packet &packet) {
                     sendData<struct packetCreatePlayer_s>(packet);
                 }                            },
                {NITWORK_POSITION_RELATIVE_BROADCAST,
                 [this](Packet &packet) {
                     sendData<struct packetPositionRelativeBroadcast_s>(packet);
                 }                            },
                {NITWORK_POSITION_ABSOLUTE_BROADCAST,
                 [this](Packet &packet) {
                     sendData<struct packetPositionAbsoluteBroadcast_s>(packet);
                 }                            },
                {NITWORK_PLAYER_DEATH,
                 [this](Packet &packet) {
                     sendData<struct packetPlayerDeath_s>(packet);
                 }                            },
                {NITWORK_MISSILE_DEATH,
                 [this](Packet &packet) {
                     sendData<struct packetMissileDeath_s>(packet);
                 }                            },
                {NITWORK_INFO_LOBBY,                  [this](Packet &packet) {
                     sendData<struct packetInfoLobby_s>(packet);
                 }},
                {NITWORK_END_GAME,
                 [this](Packet &packet) {
                     sendData<struct packetEndGame_s>(packet);
                 }},
            };
    };
} // namespace Nitwork
