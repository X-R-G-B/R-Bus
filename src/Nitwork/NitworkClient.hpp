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

            static NitworkClient &getInstance();

            using ANitwork::start;
            bool startClient(
                int port,
                const std::string &ip,
                int threadNb = DEFAULT_THREAD_NB,
                int tick     = TICKS_PER_SECOND);

            // Messages creation methods
            void addInitMsg();
            void addReadyMsg();
            void addPositionRelativeMsg(struct position_relative_s pos);
            void addPositionAbsoluteMsg(struct position_absolute_s pos);
            void addNewBulletMsg(const struct position_absolute_s &pos, const missileTypes_e &missileType);
            void addLifeUpdateMsg(n_id_t playerId, const struct health_s &life);
            void addEnemyDeathMsg(n_id_t id);

        private:
            NitworkClient();
            bool startNitworkConfig(int port, const std::string &ip) final;

            void handleBodyAction(
                const struct header_s &header,
                const boost::asio::ip::udp::endpoint &endpoint) final;

            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

        protected:

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkClient _instance; // instance of the NitworkClient (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            boost::asio::ip::udp::resolver _resolver;       // resolver used to find the server
            n_id_t _clientPacketId = 0;                     // packet id of the client
            boost::asio::ip::udp::endpoint _serverEndpoint; // endpoint of the server

            // clang-format off
            // maps that will be used to handle the actions, in order to send or receive them
            std::map<
                enum n_actionType_t,
                std::pair<handleBodyT, actionHandler>
            > _actionsHandlers = {
                {
                    INIT,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgPlayerInit_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receivePlayerInit(any, endpoint);
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
                    NEW_ALLIE,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgNewAllie_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveNewAllie(any, endpoint);
                        }
                    }
                },
                {
                    NEW_BULLET,
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
                    POSITION_ABSOLUTE_BROADCAST,
                    {
                        [this](actionHandler &handler, const struct header_s &header) {
                            handleBody<struct msgPositionAbsolute_s>(handler, header);
                        },
                        [](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::receiveBroadcastAbsolutePosition(any, endpoint);
                        }
                    }
                }
            };
            std::map<
                enum n_actionType_t,
                actionSender
                > _actionToSendHandlers = {
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
                    NEW_BULLET,
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
                }
            };
            // clang-format on
    };
} // namespace Nitwork
