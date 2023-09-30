/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Nitwork library, a portable network library
*/

#pragma once

#include <any>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <condition_variable>
#include <iostream>
#include <list>
#include <map>
#include <mutex>
#include <utility>

extern "C"
{
#include "Nitwork.h"
}

namespace Nitwork {
    constexpr int MAX_CLIENTS = 4;

    class ClientData {
        public:
            ClientData(boost::asio::ip::udp::endpoint &endpoint, std::any data)
                : endpoint(endpoint),
                  data(std::move(data))
            {
            }

            boost::asio::ip::udp::endpoint endpoint;
            std::any data;
    };
    struct packet_s {
            n_actionType_t action;
            std::any body;
    };
    class Nitwork {
        public:
            Nitwork() : _socket(_context)
            {
            }

            static Nitwork &getInstance();

            // start the server
            bool start(int port, int threadNb = 4, int tick = TICKS_PER_SECOND);

        private:
            // start the server config
            bool startServerConfig(int port);
            // start the server threads (context threads, clock thread, input
            // thread and output thread)
            bool startServerThreads(int threadNb, int tick);
            // start the context threads
            bool startContextThreads(int threadNb);
            // start the clock thread
            bool startClockThread(int tick);
            // start the input thread inside the context (post)
            void startInputHandler();
            // start the output thread inside the context (post)
            void startOutputHandler();

            // start receive handler
            void startReceiveHandler();
            // handler func for receive handler which handle the header
            void headerHandler(
                std::size_t bytes_received,
                const boost::system::error_code &error);
            // handler func for headerHandler which handle the action
            void handleBodyAction(
                const struct header_s header,
                const boost::asio::ip::udp::endpoint &endpoint);

        public:
            void stop();

            // Method which handle clock and unlock client threads each n ticks

            template <typename T>
            void sendDatasToEndpoint(
                boost::asio::ip::udp::endpoint &endpoint,
                T &datas)
            {
                _socket.send_to(
                    boost::asio::buffer(&datas, sizeof(T)),
                    endpoint);
            }

            template <typename T>
            void sendDatasToAll(T &datas)
            {
                for (auto &endpoint : _endpoints) {
                    sendDatasToEndpoint(endpoint, datas);
                }
            }

            template <typename B>
            void handleBodyDatas(
                const struct header_s &header,
                const std::function<
                    void(const std::any &, boost::asio::ip::udp::endpoint &)>
                    &handler,
                B body,
                const boost::system::error_code &error,
                const std::size_t bytes_received)
            {
                if (error) {
                    std::cerr << "Error: " << error.message() << std::endl;
                    startReceiveHandler();
                    return;
                }
                if (bytes_received != sizeof(B)) {
                    std::cerr << "Error: body not received" << std::endl;
                    startReceiveHandler();
                    return;
                }
                std::lock_guard<std::mutex> lock(_inputQueueMutex);
                ClientData clientData(_endpoint, std::any(body));
                std::cout << "adding action to queue" << std::endl;
                _actions.emplace_back(clientData, handler);
                std::cout << "action added to queue" << std::endl;
                startReceiveHandler();
            }

            template <typename B>
            void handleBody(
                const struct header_s &header,
                const std::function<
                    void(const std::any &, boost::asio::ip::udp::endpoint &)>
                    &handler)
            {
                B *body = reinterpret_cast<B *>(
                    _receiveBuffer.data() + sizeof(struct header_s)
                    + sizeof(struct action_s));
                handleBodyDatas<B>(
                    header,
                    handler,
                    *body,
                    boost::system::error_code(),
                    sizeof(B));
            }

            template <typename T>
            void sendDataFromQueue(
                const boost::asio::ip::udp::endpoint &endpoint,
                std::any &rawData)
            {
                T data = std::any_cast<T>(rawData);

                _socket.async_send_to(
                    boost::asio::buffer(&data, sizeof(T)),
                    endpoint,
                    [](const boost::system::error_code &error,
                       std::size_t bytes_sent) {
                        if (error) {
                            std::cerr << "Error: " << error.message()
                                      << std::endl;
                            return;
                        }
                        if (bytes_sent != sizeof(T)) {
                            std::cerr << "Error: package not sent" << std::endl;
                            return;
                        }
                    });
            }

            void handleInitMsg(
                const std::any &msg,
                boost::asio::ip::udp::endpoint &endpoint);

            void handleReadyMsg(
                const std::any &msg,
                boost::asio::ip::udp::endpoint &endpoint);

        protected:

        private:
            // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
            static Nitwork _instance;
            // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

            // Main vars for the server
            boost::asio::ip::udp::endpoint
                _endpoint; // The endpoint which will be used to send and
                           // receive the actions
            boost::asio::io_context _context; // The main context
            boost::asio::ip::udp::socket
                _socket; // The socket which will be used to send and receive
                         // the actions
            std::list<boost::asio::ip::udp::endpoint>
                _endpoints; // A vector of endpoints which will be used to send
                            // the actions to the clients and identify them

            // contexts threads, clock thread, input thread and output thread(s)
            std::vector<std::thread> _pool; // A pool of threads which will be
                                            // used to handle the clients
            std::thread _clockThread; // A thread for the clock which is in the
                                      // second context
            std::thread _inputThread; // A thread for the input handler which is
                                      // in the second context
            std::thread _outputThread; // A thread for the output handler which
                                       // is in the second context

            // Body handler var
            std::list<std::pair<
                ClientData,
                const std::function<
                    void(const std::any &, boost::asio::ip::udp::endpoint &)>
                    &>>
                _actions; // A list of actions which will be handled by the
                          // second context
            std::list<
                std::pair<boost::asio::ip::udp::endpoint &, struct packet_s &>>
                _outputQueue; // A queue of actions which will be sent to the
                              // clients

            // Mutex and condition variables
            std::mutex _inputQueueMutex;  // A mutex to lock the input queue
            std::mutex _outputQueueMutex; // A mutex to lock the output queue
            std::mutex
                _tickMutex; // A mutex to lock the tick condition variable
            std::condition_variable
                _tickConvVar; // A condition variable which will be handled by
                              // the clock thread to unlock the clients threads
                              // each n ticks

            // Packets vars
            struct header_s _headerPacket = {
                'N',
                0,
                0,
                0,
                0,
                'N'}; // A packet which will be used to receive the header
            struct action_s _actionPacket = {
                NO_ACTION}; // A packet which will be used to receive the action
            struct msgInit_s _initPacket = {
                'N'}; // A packet which will be used to receive the init message
            struct msgReady_s _readyPacket = {
                'N'}; // A packet which will be used to receive the ready
                      // message
            boost::asio::ip::udp::endpoint
                _clientEndpoint; // An endpoint which will be used to receive
                                 // the actions
            boost::array<char, 1024>
                _receiveBuffer; // A buffer which will be used to receive the
                                // actions

            // Actions ids
            std::array<id_t, MAX_NB_ACTION>
                _ids {}; // An array of ids which will be used to identify the
                         // actions

            // maps that will be used to handle the actions, in order to send or
            // receive them
            std::map<
                enum n_actionType_t,
                std::pair<
                    std::function<void(
                        const struct header_s &,
                        const std::function<void(
                            const std::any &,
                            boost::asio::ip::udp::endpoint &)> &)>,
                    std::function<void(
                        const std::any &,
                        boost::asio::ip::udp::endpoint &)>>>
                _actionsHandlers = {
                    {INIT,
                     std::make_pair(
                         std::function<void(
                             const struct header_s &,
                     const std::function<void(
                                 const std::any &,
                     boost::asio::ip::udp::endpoint &)> &)>(
                             std::bind(
                                 &Nitwork::handleBody<struct msgInit_s>,
                     this, std::placeholders::_1,
                     std::placeholders::_2)),
                     std::function<void(
                             const std::any &,
                     boost::asio::ip::udp::endpoint &)>(
                             std::bind(
                                 &Nitwork::handleInitMsg,
                     this, std::placeholders::_1,
                     std::placeholders::_2)))},
                    {READY,
                     std::make_pair(
                         std::function<void(
                             const struct header_s &,
                     const std::function<void(
                                 const std::any &,
                     boost::asio::ip::udp::endpoint &)> &)>(
                             std::bind(
                                 &Nitwork::handleBody<struct msgReady_s>,
                     this, std::placeholders::_1,
                     std::placeholders::_2)),
                     std::function<void(
                             const std::any &,
                     boost::asio::ip::udp::endpoint &)>(
                             std::bind(
                                 &Nitwork::handleReadyMsg,
                     this, std::placeholders::_1,
                     std::placeholders::_2)))}
            };
            std::map<
                enum n_actionType_t,
                std::function<
                    void(const boost::asio::ip::udp::endpoint &, std::any &)>>
                _actionToSendHandlers = {
                    {INIT,
                     std::function<void(
                         const boost::asio::ip::udp::endpoint &,
                     std::any &)>(
                         std::bind(
                             &Nitwork::sendDataFromQueue<struct msgInit_s>,
                     this, std::placeholders::_1,
                     std::placeholders::_2))},
                    {READY,
                     std::function<void(
                         const boost::asio::ip::udp::endpoint &,
                     std::any &)>(
                         std::bind(
                             &Nitwork::sendDataFromQueue<struct msgReady_s>,
                     this, std::placeholders::_1,
                     std::placeholders::_2))}
            };
    };
} // namespace Nitwork
