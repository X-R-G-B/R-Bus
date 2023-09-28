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

#define MAX_CLIENTS 4

namespace Nitwork {
    struct clientData_s {
        boost::asio::ip::udp::endpoint &endpoint;
        std::any data;
    };
    struct packet_s {
        n_actionType_t action;
        std::any body;
    };
    class Nitwork {
        public:
            Nitwork()
                : _socket(_context) {}

            static Nitwork &getInstance();

            // start the server
            bool start(int port);

            void stop();

            void inputHandler();

            // Method which handle clock and unlock client threads each n ticks
            void clockThread(int tick);

            void clientsDatasHandler();

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
            void handleBodyDatas(const struct header_s &header, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &handler, B body, const boost::system::error_code& error, const std::size_t bytes_received) {
                if (error) {
                    std::cerr << "Error: " << error.message() << std::endl;
                    return;
                }
                if (bytes_received != sizeof(B)) {
                    std::cerr << "Error: body not received" << std::endl;
                    return;
                }
                clientData_s clientData = { _endpoint, std::any(body) };
                _actions.emplace_back(clientData, handler);
            }

            template<typename B>
            void handleBody(const struct header_s &header, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &handler) {
                B body;

                _socket.async_receive_from(boost::asio::buffer(&body, sizeof(B)), _endpoint, boost::bind(&Nitwork::handleBodyDatas<B>, this, header, handler, body, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }

            void handleBodyActionData(const struct action_s &action, const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint, const boost::system::error_code& error, std::size_t bytes_received);

            void handleBodyAction(const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint);

            void readDataFromEndpointHandler(const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint, std::size_t bytes_received, const boost::system::error_code& error);

            void readDataFromEndpoint();

            template<typename T>
            void sendDataFromQueue(const boost::asio::ip::udp::endpoint &endpoint, std::any &rawData) {
                T data = std::any_cast<T>(rawData);

                _socket.async_send_to(boost::asio::buffer(&data, sizeof(T)), endpoint, [](const boost::system::error_code& error, std::size_t bytes_sent) {
                    if (error) {
                        std::cerr << "Error: " << error.message() << std::endl;
                        return;
                    }
                    if (bytes_sent != sizeof(T)) {
                        std::cerr << "Error: package not sent" << std::endl;
                        return;
                    }
                });
            }

            void outputHandler();

            void handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint);

            bool isRunning() const;
        protected:
        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static Nitwork _instance;
            bool _isRunning = false;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
            boost::asio::io_context _context; // second context which will handle the outputs (handle actions and send it, each n ticks
            std::thread _inputThread; // A thread for the first context
            std::thread _clockThread; // A thread for the clock which is in the second context
            std::thread _outputThread; // A thread which will send the actions to the clients from the output queue

            // Body handler var
            std::list<std::pair<struct clientData_s, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &>> _actions; // A list of actions which will be handled by the second context
            std::list<std::pair<boost::asio::ip::udp::endpoint &, struct packet_s &>> _outputQueue; // A queue of actions which will be sent to the clients

            std::mutex _queueMutex; // A mutex to lock the queue which will be used by both contexts
            std::condition_variable _queueCondVar; // A condition variable to wait for the queue to be used by the second context
            std::list<boost::asio::ip::udp::endpoint> _endpoints; // A vector of endpoints which will be used to send the actions to the clients and identify them
            std::array<id_t, MAX_NB_ACTION> _ids{}; // An array of ids which will be used to identify the actions
            boost::asio::ip::udp::socket _socket; // The socket which will be used to send and receive the actions
            boost::asio::ip::udp::endpoint _endpoint; // The endpoint which will be used to send and receive the actions

            std::map<
                enum n_actionType_t,
                std::pair<
                    std::function<void(const struct header_s &, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &)>,
                    std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)>
                    >
                > _actionsHandlers = {
                    {INIT,
                        std::make_pair(
                            std::function<void(const struct header_s &, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &)>(std::bind(&Nitwork::handleBody<struct msgInit_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)>(std::bind(&Nitwork::handleInitMsg, this, std::placeholders::_1, std::placeholders::_2))
                        )
                    },
                    {READY,
                        std::make_pair(
                            std::function<void(const struct header_s &, const std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)> &)>(std::bind(&Nitwork::handleBody<struct msgReady_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &, boost::asio::ip::udp::endpoint &)>(std::bind(&Nitwork::handleReadyMsg, this, std::placeholders::_1, std::placeholders::_2))
                        )
                    }
                };
            std::map<
                enum n_actionType_t,
                std::function<void(const boost::asio::ip::udp::endpoint &, std::any &)>
                > _actionToSendHandlers = {
                    {INIT,
                        std::function<void(const boost::asio::ip::udp::endpoint &, std::any &)>(std::bind(&Nitwork::sendDataFromQueue<struct msgInit_s>, this, std::placeholders::_1, std::placeholders::_2))
                    },
                    {READY,
                        std::function<void(const boost::asio::ip::udp::endpoint &, std::any &)>(std::bind(&Nitwork::sendDataFromQueue<struct msgReady_s>, this, std::placeholders::_1, std::placeholders::_2))
                    }
                };


    };
}
