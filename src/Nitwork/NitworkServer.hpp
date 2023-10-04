/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkServer
*/

#pragma once

#include "ANitwork.hpp"

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

            bool startNitworkConfig(int port, const std::string &ip) final;

            void handleBodyAction(const boost::asio::ip::udp::endpoint &endpoint) final;

            /* Messages creation methods */
            void addStarGameMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId);

        private:
            NitworkServer() = default;

            [[nodiscard]] const std::map<enum n_actionType_t, actionHandler> &
            getActionToSendHandlers() const final;

            bool isClientAlreadyConnected(boost::asio::ip::udp::endpoint &endpoint) const;

            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkServer _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            std::list<boost::asio::ip::udp::endpoint>
                _endpoints; // A vector of endpoints which will be used to send the actions to the clients
                            // and identify them

            // maps that will be used to handle the actions, in order to send or receive them
            std::map<enum n_actionType_t, std::pair<handleBodyT, actionHandler>> _actionsHandlers = {
                {
                     INIT,
                     std::make_pair(
                         handleBodyT([this](actionHandler &actionHandler) {
                             handleBody<struct msgInit_s>(actionHandler);
                         }),
                         actionHandler([this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                             handleInitMsg(msg, endpoint);
                         })
                     )
                },
                {
                     READY,
                     std::make_pair(
                         handleBodyT([this](actionHandler &actionHandler) {
                             handleBody<struct msgReady_s>(actionHandler);
                         }),
                         actionHandler([this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                             handleReadyMsg(msg, endpoint);
                         })
                     )
                },
            };
            std::map<enum n_actionType_t, actionHandler> _actionToSendHandlers = {
                {
                     START_GAME,
                     actionHandler([this](std::any &any, boost::asio::ip::udp::endpoint &endpoint) {
                            sendData<struct packetMsgStartGame_s>(any, endpoint);
                     })
                }
            };
    };
} // namespace Nitwork
