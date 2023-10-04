/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** NitworkClient
*/

#pragma once

#include "ANitwork.hpp"

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
            bool start(
                int port,
                int threadNb          = DEFAULT_THREAD_NB,
                int tick              = TICKS_PER_SECOND,
                const std::string &ip = "") final;

            bool startNitworkConfig(int port, const std::string &ip) final;

            void handleBodyAction(const boost::asio::ip::udp::endpoint &endpoint) final;

            // Get bytes of packets ids receives
            n_idsReceived_t getIdsReceived();
            // Messages creation methods
            void addInitMsg();
            void addReadyMsg();

        private:
            NitworkClient();
            [[nodiscard]] const std::map<enum n_actionType_t, actionHandler> &
            getActionToSendHandlers() const final;

            void handleStartGame(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

        protected:

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkClient _instance; // instance of the NitworkClient (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            boost::asio::ip::udp::resolver _resolver; // resolver used to find the server
            n_id_t _clientPacketId = 0;               // packet id of the client

            // clang-format off
            // maps that will be used to handle the actions, in order to send or receive them
            std::map<
                enum n_actionType_t,
                std::pair<handleBodyT, actionHandler>
            > _actionsHandlers = {
                {
                    START_GAME,
                    std::make_pair(
                        handleBodyT([this](actionHandler &handler) {
                            handleBody<struct msgStartGame_s>(handler);
                        }),
                        actionHandler([this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            handleStartGame(any, endpoint);
                        })
                    ),
                }
            };
            std::map<
                enum n_actionType_t,
                actionHandler
                > _actionToSendHandlers = {
                {
                    INIT,
                    actionHandler([this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            sendData<struct packetMsgInit_s>(any, endpoint);
                        }
                    )
                },
                {
                    READY,
                    actionHandler(
                        [this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            sendData<struct packetMsgReady_s>(any, endpoint);
                        }
                    )
                }
            };
            // clang-format on
    };
} // namespace Nitwork
