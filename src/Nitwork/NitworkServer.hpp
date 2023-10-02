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
            NitworkServer();
            ~NitworkServer() = default;

            static NitworkServer &getInstance();

            bool start(int port, int threadNb = DEFAULT_THREAD_NB, int tick = TICKS_PER_SECOND, const std::string &ip = "") final;

            bool startNitworkConfig(int port, const std::string &ip) final;

            void handleBodyAction(
                const struct header_s header,
                const boost::asio::ip::udp::endpoint &endpoint) final;
        private:
            void handleHeaderPacketsIds(const struct header_s &);

            [[nodiscard]] const std::map<
                enum n_actionType_t,
                actionHandler
            >& getActionToSendHandlers() const final;

            void handleInitMsg(
                const std::any &msg,
                boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(
                const std::any &msg,
                boost::asio::ip::udp::endpoint &endpoint);
        protected:
        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static NitworkServer
                _instance; // instance of the NitworkServer (singleton)
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            std::list<boost::asio::ip::udp::endpoint> _endpoints; // A vector of endpoints which will be used to send the actions to the clients and identify them

            // maps that will be used to handle the actions, in order to send or receive them
            std::map<
                enum n_actionType_t,
                std::pair<handleBodyT, actionHandler>
            > _actionsHandlers = {
                {
                    INIT,
                    std::make_pair(
                        handleBodyT(
                            std::bind(
                                &NitworkServer::handleBody<struct msgInit_s>,
                                this, std::placeholders::_1,
                                std::placeholders::_2
                            )
                        ),
                        actionHandler(
                            std::bind(
                                &NitworkServer::handleInitMsg,
                                this, std::placeholders::_1,
                                std::placeholders::_2
                            )
                        )
                    )
                },
                {
                     READY,
                     std::make_pair(
                         handleBodyT(
                             std::bind(
                                &NitworkServer::handleBody<struct msgReady_s>,
                                this, std::placeholders::_1,
                                std::placeholders::_2
                            )
                         ),
                         actionHandler(
                             std::bind(
                                 &NitworkServer::handleReadyMsg,
                                 this, std::placeholders::_1,
                                 std::placeholders::_2
                             )
                         )
                     )
                },
            };
            std::map<
                enum n_actionType_t,
                actionHandler
            > _actionToSendHandlers = {
                    {
                         INIT,
                         actionHandler(
                             std::bind(
                                 &ANitwork::sendData<struct msgInit_s>,
                                 this, std::placeholders::_1,
                                 std::placeholders::_2
                             )
                        )
                    },
                    {
                        READY,
                        actionHandler(
                            std::bind(
                                &ANitwork::sendData<struct msgReady_s>,
                                this, std::placeholders::_1,
                                std::placeholders::_2
                            )
                        )
                    }
            };
    };
}