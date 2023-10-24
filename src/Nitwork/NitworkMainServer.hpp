/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkMainServer
*/

#pragma once

#include "MainNitwork.h"
#include "ANitwork.hpp"
#include "MainServerNetwork.hpp"

namespace Nitwork {
    class NitworkMainServer : public ANitwork {
    public:
        ~NitworkMainServer() override              = default;
        NitworkMainServer(const NitworkMainServer &)   = delete;
        NitworkMainServer(const NitworkMainServer &&)  = delete;
        void operator=(const NitworkMainServer &)  = delete;
        void operator=(const NitworkMainServer &&) = delete;

        /**
         * @brief The instance of the server
         * @note This is a singleton
         */
        static NitworkMainServer &getInstance();

        /**
         * @brief Start the server
         * @param port The port to listen
         * @param nbPlayer The max number of player
         * @param threadNb The number of thread to use
         * @param tick The tick rate of the server
         * @return true if the server started, false otherwise
         * @note This method is thread safe
         */
        bool startServer(int port, int nbPlayer, int threadNb = DEFAULT_THREAD_NB, int tick = TICKS);

    /* Handlers methods of the received actions */
    private:
        void handleInitMsg(std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

    private:
        NitworkMainServer() = default;

        /**
         * @brief Start the network configuration
         * @param port The port to listen
         * @param ip The ip to listen
         * @return true if the network started, false otherwise
         */
        bool startNitworkConfig(int port, const std::string &ip) final;

        /**
         * @brief Get the action handlers to handle the received actions from the clients
         * @return The action handlers
         * @note This method is thread safe
         */
        [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &getActionToSendHandlers() const final;

        /**
         * @brief Handle the body of the received packet
         * @param header The header of the packet
         * @param endpoint The endpoint of the sender
         * @note This method is thread safe
         */
        void handleBodyAction(
            const struct header_s &header,
            const boost::asio::ip::udp::endpoint &endpoint) final;

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static NitworkMainServer _instance; // instance of the NitworkServer (singleton)
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
        unsigned int _maxNbPlayer = 0; // max number of players
        std::unordered_map<boost::asio::ip::udp::endpoint, n_id_t> _playersIds;
        // clang-format off

        std::map<enum n_actionType_t, std::pair<handleBodyT, actionHandler>> _actionsHandlers = {
            {
                INIT,
                {
                    [this](actionHandler &actionHandler, const struct header_s &header) {
                        handleBody<struct msgInit_s>(actionHandler, header);
                    },
                    [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                        handleInitMsg(msg, endpoint);
                    }
                }
            },
            {
                LIST_LOBBY,
                {
                    [this](actionHandler &actionHandler, const struct header_s &header) {
                        handleBody<struct msgNewLobby_s>(actionHandler, header);
                    },
                    [this](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                        Systems::handleNewLobbyMsg(msg, endpoint);
                    }
                }
            }
        };
        std::map<enum n_actionType_t, actionSender> _actionToSendHandlers = {};
        // clang-format on
    };
} // namespace Nitwork
