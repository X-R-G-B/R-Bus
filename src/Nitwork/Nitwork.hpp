/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Nitwork library, a portable network library
*/

#pragma once

#include <iostream>
#include <list>
#include <map>
#include <any>
#include <condition_variable>
#include <mutex>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/asio.hpp>
#include "Nitwork.h"

#define MAX_PLAYERS 4

namespace Nitwork {
    class Nitwork {
        public:
            Nitwork()
                : _socket(_context) {}

            static Nitwork &getInstance();

            // start the server
            bool Start(int port);

            bool Stop();

            void ContinuousReception();

            // Method which handle clock and unlock client threads each n ticks
            void ClockThread(int tick);

            void ClientsDatasHandler();

            template<typename T>
            void sendDatasToEndpoint(boost::asio::ip::udp::endpoint &endpoint, T &datas) {
                _socket.send_to(boost::asio::buffer(&datas, sizeof(T)), endpoint);
            }

            template<typename T>
            void sendDatasToAll(T &datas) {
                for (auto &endpoint : _endpoints) {
                    sendDatasToEndpoint(endpoint, datas);
                }
            }

            template<typename B>
            void handleBodyDatas(const struct header_s &header, const std::function<void(std::any)> &handler, B body, const boost::system::error_code& error, const std::size_t bytes_received) {
                if (error) {
                    std::cerr << "Error: " << error.message() << std::endl;
                    return;
                }
                if (bytes_received != sizeof(B)) {
                    std::cerr << "Error: body not received" << std::endl;
                    return;
                }
                _actions.emplace_back(body);
                _actionsHandler.emplace_back(handler);
            }

            template<typename B>
            void handleBody(const struct header_s &header, const std::function<void(const std::any &)> &handler) {
                B body;

                _socket.async_receive_from(boost::asio::buffer(&body, sizeof(B)), _endpoint, boost::bind(&Nitwork::handleBodyDatas<B>, this, header, handler, body, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }

            void handleBodyActionDatas(const struct action_s &action, const struct header_s &header, const boost::system::error_code& error, std::size_t bytes_received);

            void handleBodyAction(const struct header_s &header);

            void readDataFromEndpoint(boost::asio::ip::udp::endpoint &endpoint);

            void handleInitMsg(const std::any &msg);

            void handleReadyMsg(const std::any &msg);

        protected:
        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static Nitwork _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            boost::asio::io_context _context; // second context which will handle the outputs (handle actions and send it, each n ticks
            std::thread _inputThread; // A thread for the first context
            std::thread _clockThread; // A thread for the clock which is in the second context
            std::array<std::thread, MAX_PLAYERS> _outputThreads; // A thread for each player which is in the second context

            // Body handler vars
            std::list<std::any> _actions; // A list of actions which will be handled by the second context
            std::list<std::function<void(const std::any &)>> _actionsHandler; // A list of functions which will be used to handle the actions

            std::mutex _queueMutex; // A mutex to lock the queue which will be used by both contexts
            std::condition_variable _queueCondVar; // A condition variable to wait for the queue to be used by the second context
            std::vector<boost::asio::ip::udp::endpoint> _endpoints; // A vector of endpoints which will be used to send the actions to the clients and identify them
            std::array<id_t, MAX_NB_ACTION> _ids{}; // An array of ids which will be used to identify the actions
            boost::asio::ip::udp::socket _socket; // The socket which will be used to send and receive the actions
            boost::asio::ip::udp::endpoint _endpoint; // The endpoint which will be used to send and receive the actions

            const std::map<
                enum n_actionType_t,
                std::pair<
                    std::function<void(const struct header_s &, const std::function<void(const std::any &)> &)>,
                    std::function<void(const std::any &)>
                    >
                > _actionsHandlers = {
                    {INIT,
                        std::make_pair(
                            std::function<void(const struct header_s &, const std::function<void(const std::any &)> &)>(std::bind(&Nitwork::handleBody<struct msgInit_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &)>(std::bind(&Nitwork::handleInitMsg, this, std::placeholders::_1))
                        )
                    },
                    {READY,
                        std::make_pair(
                            std::function<void(const struct header_s &, const std::function<void(const std::any &)> &)>(std::bind(&Nitwork::handleBody<struct msgReady_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &)>(std::bind(&Nitwork::handleReadyMsg, this, std::placeholders::_1))
                        )
                    }
                };


    };
}
