/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkMainServer
*/

#pragma once

#include "ANitwork.hpp"
#include "MainServerNetwork.hpp"

namespace Nitwork {
    class NitworkMainServer : public ANitwork {
        public:
            ~NitworkMainServer() override                 = default;
            NitworkMainServer(const NitworkMainServer &)  = delete;
            NitworkMainServer(const NitworkMainServer &&) = delete;
            void operator=(const NitworkMainServer &)     = delete;
            void operator=(const NitworkMainServer &&)    = delete;

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
            bool startServer(
                int port,
                int nbPlayer = MAX_MAIN_SERVER_CLIENT,
                int threadNb = DEFAULT_THREAD_NB,
                int tick     = TICKS);

            /**
             * @brief Stop the server
             */
            void stop() final;

            /**
             * @brief Send a message to the client to let him know he's connected
             * @param endpoint The endpoint of the client
             */
            void sendConnectMainServerResp(const boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Send the list of lobbies to the client
             * @param endpoint The endpoint of the client
             * @param lobbies The list of lobbies (max 5)
             */
            void sendListLobby(
                const boost::asio::ip::udp::endpoint &endpoint,
                const std::vector<struct lobby_s> &lobbies);

            /**
             * @brief Create a lobby
             * @param nbPlayer The number of player
             * @param name The name of the lobby
             * @param ownerIp The ip of the owner
             * @param ownerPort The port of the owner
             * @param gameType The type of the game (determine which system is active during the game)
             */
            void createLobby(unsigned int maxNbPlayer, const std::string &name, enum gameType_e gameType);

            /**
             * @brief Get the list of lobbies
             * @return The list of lobbies
             */
            const std::vector<struct lobby_s> &getLobbies() const;

            /**
             * @brief Add a lobby to the list of lobbies
             * @param lobby
             */
            void addLobby(const struct lobby_s &lobby);


            /**
             * @brief Set the ip of the MainServer
             * @param ip The ip of the MainServer
             */
            void setIpOfMainServer(const std::string &ip);

            /**
             * @brief Get the available ips
             * @return the available ips
             */
            std::vector<std::string> getAvailableIps() const;

        private:
            /**
             * @brief Handle the init message
             * @param msg The message
             * @param endpoint The endpoint of the sender
             */
            void handleInitMsg(std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            /**
             * @brief Fork a process and create a lobby with execl call to the lobby binary
             * @param nbPlayer The number of player
             * @param name The name of the lobby
             * @param ownerIp The ip of the owner
             * @param ownerPort The port of the owner
             */
            void forkProcessAndCreateLobby(
                unsigned int maxNbPlayer,
                enum gameType_e gameType,
                const std::string &name,
                const std::string &ownerIp,
                int ownerPort);

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
            [[nodiscard]] const std::map<enum n_actionType_t, actionSender> &
            getActionToSendHandlers() const final;

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
            unsigned int _maxNbPlayer = 0;        // max number of players
            std::vector<int> _lobbyPids;          // pid of the lobbies
            std::vector<struct lobby_s> _lobbies; // list of lobbies
            std::string _ip;                      // ip of the MainServer
            // clang-format off

            std::map<enum n_actionType_t, std::pair<handleBodyT, actionHandler>> _actionsHandlers = {
                {
                    CONNECT_MAIN_SERVER,
                    {
                        [this](actionHandler &actionHandler, const struct header_s &header) {
                            handleBody<struct msgConnectMainServer_s>(actionHandler, header);
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
                            handleBody<struct msgRequestListLobby_s>(actionHandler, header);
                        },
                        [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::handleRequestListLobbyMsg(msg, endpoint);
                        }
                    }
                },
                {
                    CREATE_LOBBY,
                    {
                        [this](actionHandler &actionHandler, const struct header_s &header) {
                            handleBody<struct msgCreateLobby_s>(actionHandler, header);
                        },
                        [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::handleCreateLobbyMsg(msg, endpoint);
                        }
                    }
                },
                {
                    INFO_LOBBY,
                    {
                        [this](actionHandler &actionHandler, const struct header_s &header) {
                            handleBody<struct msgInfoLobby_s>(actionHandler, header);
                        },
                        [](std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
                            Systems::handleInfoLobbyMsg(msg, endpoint);
                        }
                    }
                }
            };
            std::map<enum n_actionType_t, actionSender> _actionToSendHandlers = {
                {
                    LIST_LOBBY,
                    [this](Packet &packet) {
                        sendData<struct packetListLobby_s>(packet);
                    }
                },
                {
                    CONNECT_MAIN_SERVER_RESP,
                    [this](Packet &packet) {
                        sendData<struct packetConnectMainServerResp_s>(packet);
                    }
                }
            };
            // clang-format on
    };
} // namespace Nitwork
