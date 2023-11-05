/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** NitworkClient
*/

#pragma once

#include "ANitwork.hpp"
#include "ClientNetwork.hpp"

namespace Nitwork {
    class NitworkClient : public ANitwork {
        public:
            ~NitworkClient() override              = default;
            NitworkClient(const NitworkClient &)   = delete;
            NitworkClient(const NitworkClient &&)  = delete;
            void operator=(const NitworkClient &)  = delete;
            void operator=(const NitworkClient &&) = delete;

            /**
             * @brief Get the instance of the NitworkClient
             * @return The instance of the NitworkClient
             */
            static NitworkClient &getInstance();

            /**
             * @brief Start the client
             */
            using ANitwork::start;
            bool startClient(int threadNb = DEFAULT_THREAD_NB, int tick = TICKS);

            /* Messages creation methods */
            /**
             * @brief Connect the client to the main server
             * @param ip The ip of the main server
             * @param port The port of the main server
             */
            bool connectMainServer(const std::string &ip, n_port_t port);

            /**
             * @brief Connect the client to the lobby
             * @param ip
             * @param port
             */
            void connectLobby(const std::string &ip, n_port_t port);

            /**
             * @brief Disconnect the client from the main server
             */
            void disconnectLobby();

            /**
             * @brief Add a new init message to the packet
             * in order to create a new player when the server respond
             */
            void addInitMsg();

            /**
             * @brief Add a new ready message to the packet
             * in order to tell the server that the player is ready
             * and can start the game
             */
            void addReadyMsg();

            /**
             * @brief Add a new position relative message to the packet
             * @param pos The position of the new player
             */
            void addPositionRelativeMsg(struct position_relative_s pos);

            /**
             * @brief Add a new position absolute message to the packet
             * @param pos The position of the new player
             */
            void addPositionAbsoluteMsg(struct position_absolute_s pos);

            /**
             * @brief Add a new bullet message to the packet
             * @param pos The position of the new bullet
             * @param missileType The type of the new bullet
             */
            void addNewBulletMsg(const struct position_absolute_s &pos, const missileTypes_e &missileType);

            /**
             * @brief Add a new player message to the packet
             * @param playerId The id of the new player
             * @param life The life of the new player
             */
            void addLifeUpdateMsg(n_id_t playerId, const struct health_s &life);

            /**
             * @brief Add a new enemy message to the packet
             * @param id The id of the new enemy
             */
            void addEnemyDeathMsg(n_id_t id);

            /**
             * @brief Add a player death message to the packet
             * @param id The id of the dead player
             */
            void addPlayerDeathMsg(n_id_t id);
            void addMissileDeathMsg(n_id_t id);

            /**
             * @brief Add a connect main server message to the packet
             */
            void addListLobbyMsg();

            /**
             * @brief Add a create lobby message to the packet
             * @param name The name of the lobby
             * @param gameType The type of the game
             * @param maxNbPlayer The max number of player in the lobby
             */
            void
            addCreateLobbyMsg(const std::string &name, enum gameType_e gameType, unsigned int maxNbPlayer);

            /**
             * @brief Create a server
             * @param port port of the server
             */
            void createForkedServer(const std::string &port);

            /**
             * @brief Stop the server
             */
            void stop() final;

            /* Private connection methods */

        private:
            /**
             * @brief Set the main endpoint (the main server)
             * if the client is already connected to a main server, it will be disconnected
             * @param ip The ip of the main server
             * @param port The port of the main server
             */
            void setMainEndpoint(const std::string &ip, n_port_t port);

            /**
             * @brief Set the lobby endpoint (the game server)
             * if the client is already connected to a game server (lobby), it will be disconnected
             * @param ip The ip of the server
             * @param port The port of the server
             */
            void setLobbyEndpoint(const std::string &ip, n_port_t port);

            /**
             * @brief Send a connection msg to connect to the main server
             */
            void addConnectMainServerMsg();

            /**
             * @brief Send a connection msg to connect to the lobby
             */
            void addConnectLobbyMsg();

        private:
            /**
             * @brief Constructor of the NitworkClient
             */
            NitworkClient();

            /**
             * @brief Start the network configuration
             * @param port The port of the client
             * @param ip The ip of the client
             * @return True if the network configuration is a success, false otherwise
             */
            bool startNitworkConfig(int port, const std::string &ip) final;

            /**
             * @brief Handle the body of the packet
             * @param header The header of the packet
             * @param endpoint The endpoint of the packet
             */
            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            /**
             * @brief Get the action handlers map
             * it will be used to handle the actions, in order to send them
             * @return The action handlers map
             */
            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

            /**
             * @brief Add a new endpoint to the client (like a lobby/main server)
             * @param ip The ip of the endpoint
             * @param port The port of the endpoint
             * @return The endpoint
             */
            boost::asio::ip::udp::endpoint &addEndpoint(const std::string &ip, n_port_t port);

            /**
             * @brief Remove an endpoint from the client (like a lobby/main server)
             * @param ip The ip of the endpoint
             * @param port The port of the endpoint
             */
            void removeEndpoint(const std::string &ip, n_port_t port);

        protected:

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            /**
             * @brief The instance of the NitworkClient (singleton)
             */
            static NitworkClient _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

            /**
             * @brief The resolver used to find the server
             */
            boost::asio::ip::udp::resolver _resolver;

            /**
             * @brief The endpoint of the main server
             */
            boost::asio::ip::udp::endpoint _mainServerEndpoint;

            /**
             * @brief The endpoint of the server
             */
            boost::asio::ip::udp::endpoint _serverEndpoint;

            std::vector<int> _serverPids; // pid of the servers

            // clang-format off
            /**
             * @brief Map that will be used to handle the actions, in order to send or receive them
             */
            std::map<
                enum n_actionType_t,
                std::pair<handleBodyT, actionHandler>
            > _actionsHandlers = {
                {
                    CONNECT_LOBBY_RESP,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgConnectLobbyResp_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveConnectLobbyResp(any, endpoint);
                        }
                    },
                },
                {
                    INIT,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgCreatePlayer_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveNewPlayer(any, endpoint);
                        }
                    },
                },
                {
                    START_WAVE,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgStartWave_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::handleStartWave(any, endpoint);
                        }
                    },
                },
                {
                    LIFE_UPDATE,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgLifeUpdate_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveLifeUpdate(any, endpoint);
                        }
                    }
                },
                {
                    ENEMY_DEATH,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgEnemyDeath_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveEnemyDeath(any, endpoint);
                        }
                    }
                },
                {
                    NEW_ENEMY,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgNewEnemy_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveNewEnemy(any, endpoint);
                        }
                    }
                },
                {
                    NEW_PLAYER,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgCreatePlayer_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveNewPlayer(any, endpoint);
                        }
                    }
                },
                {
                    NEW_MISSILE,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgNewBullet_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveNewBullet(any, endpoint);
                        }
                    }
                },
                {
                    POSITION_RELATIVE_BROADCAST,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgPositionRelativeBroadcast_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveRelativePosition(any, endpoint);
                        }
                    },
                },
                {
                    POSITION_ABSOLUTE_BROADCAST,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgPositionAbsoluteBroadcast_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveBroadcastAbsolutePosition(any, endpoint);
                        }
                    }
                },
                {
                    PLAYER_DEATH,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgPlayerDeath_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receivePlayerDeath(any, endpoint);
                        }
                    }
                },
                {
                    MISSILE_DEATH,
                    {
                         [this](actionHandler &handler, const struct header_s &header) {
                           handleBody<struct msgMissileDeath_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveMissileDeath(any, endpoint);
                        }
                    }
                },
                {
                    LIST_LOBBY,
                    {
                        [this](actionHandler &actionHandler, const struct header_s &header) {
                            handleBody<struct msgLobbyInfo_s>(actionHandler, header);
                        },
                        [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::handleNewLobbyMsg(msg, endpoint);
                        }
                    }
                },
                {
                    CONNECT_MAIN_SERVER_RESP,
                    {
                        [this](actionHandler &actionHandler, const struct header_s &header) {
                            handleBody<struct msgConnectMainServerResp_s>(actionHandler, header);
                        },
                        [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveConnectMainServerResp(msg, endpoint);
                        }
                    }
                },
                {
                    END_GAME,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgEndGame_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveEndGame(any, endpoint);
                        }
                    }
                }
            };
            std::map<
                enum n_actionType_t,
                actionSender
                > _actionToSendHandlers = {
                {
                    CONNECT_LOBBY,
                    [this](Packet &packet) {
                        sendData<struct packetConnectLobby_s>(packet);
                    }
                },
                {
                    INIT,
                    [this](Packet &packet) {
                            sendData<struct packetMsgInit_s>(packet);
                        }
                },
                {
                    READY,
                        [this](Packet &packet) {
                            sendData<struct packetMsgReady_s>(packet);
                        }
                },
                {
                    POSITION_RELATIVE,
                        [this](Packet &packet) {
                            sendData<struct packetPositionRelative_s>(packet);
                        }
                },
                {
                    POSITION_ABSOLUTE,
                        [this](Packet &packet) {
                            sendData<struct packetPositionAbsolute_s>(packet);
                        }
                },
                {
                    NEW_MISSILE,
                        [this](Packet &packet) {
                            sendData<struct packetNewBullet_s>(packet);
                        }
                },
                {
                    LIFE_UPDATE,
                        [this](Packet &packet) {
                            sendData<struct packetLifeUpdate_s>(packet);
                        }
                },
                {
                    ENEMY_DEATH,
                        [this](Packet &packet) {
                            sendData<struct packetEnemyDeath_s>(packet);
                        }
                },
                {
                    PLAYER_DEATH,
                        [this](Packet &packet) {
                            sendData<struct packetPlayerDeath_s>(packet);
                        }
                },
                {
                    CONNECT_MAIN_SERVER,
                    [this](Packet &packet) {
                        sendData<struct packetConnectMainServer_s>(packet);
                    }
                },
                {
                    LIST_LOBBY,
                    [this](Packet &packet) {
                        sendData<struct packetRequestListLobby_s>(packet);
                    }
                },
                {
                    CREATE_LOBBY,
                    [this](Packet &packet) {
                        sendData<struct packetCreateLobby_s>(packet);
                    }
                },
                {
                    DISCONNECT_LOBBY,
                    [this](Packet &packet) {
                        sendData<struct packetDisconnectLobby_s>(packet);
                    }
                }
            };
            // clang-format on
    };
} // namespace Nitwork
