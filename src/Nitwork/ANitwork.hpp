/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ANitwork
*/

#pragma once

#include <iostream>
#include <list>
#include <mutex>
#include <boost/asio/placeholders.hpp>
#include <boost/bind/bind.hpp>
#include <condition_variable>
#include "INitwork.hpp"

namespace Nitwork {
    constexpr int MAX_PACKET_SIZE = 1024;

    class ANitwork : public INitwork {
        public:
            virtual ~ANitwork() = default;
            ANitwork(const ANitwork &) = delete;
            ANitwork(const ANitwork &&) = delete;
            void operator=(const ANitwork &) = delete;
            void operator=(const ANitwork &&) = delete;

            // start the NitworkServer
            bool start(int port, int threadNb, int tick, const std::string &ip = "") override;

            void stop() override;
            // send data to the endpoint with the given data
            template <typename T>
            void sendData(
                std::any &rawData,
                boost::asio::ip::udp::endpoint &endpoint)
            {
                if (rawData.type() != typeid(T)) {
                    std::cerr << "Error: invalid type" << std::endl;
                    return;
                }
                if (sizeof(T) > MAX_PACKET_SIZE) {
                    std::cerr << "Error: package too big" << std::endl;
                    return;
                }
                T data = std::any_cast<T>(rawData);

                std::cout << "Sending data to " << endpoint.address().to_string()
                          << ":" << endpoint.port() << std::endl;
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


        protected:
            ANitwork();

            /* Getters / Setters */
            n_id_t getPacketID();
            void addPacketToSend(const boost::asio::ip::udp::endpoint &, const struct packet_s &);
            void handlePacketIdsReceived(const struct header_s &header);

            void startReceiveHandler() final;
            // handler func for receive handler which handle the header
            template <typename B>
            void handleBody(const actionHandler &handler)
            {
                auto *body = reinterpret_cast<B *>(
                    _receiveBuffer.data() + sizeof(struct header_s));
                handleBodyDatas<B>(
                    handler,
                    *body,
                    boost::system::error_code(),
                    sizeof(B));
            }


        private:
            // start the NitworkServer threads (context threads, clock thread, input thread and output thread)
            bool startNitworkThreads(int threadNb, int tick) final;
            // start the context threads
            bool startContextThreads(int threadNb) final;
            // start the clock thread
            bool startClockThread(int tick) final;
            // start the input thread inside the context (post)
            void startInputHandler() final;
            // start the output thread inside the context (post)
            void startOutputHandler() final;
            // start receive handler
            void headerHandler(
                std::size_t bytes_received,
                const boost::system::error_code &error) final;
            bool isAlreadyReceived(n_id_t id);

            // handler func for receive handler which handle the action
            template <typename B>
            void handleBodyDatas(
                const actionHandler &handler,
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
                SenderData senderData(_senderEndpoint, std::any(body));
                _actions.emplace_back(senderData, handler);
                startReceiveHandler();
            }
            void addPacketToSentPackages(const std::pair<boost::asio::ip::basic_endpoint<boost::asio::ip::udp>, packet_s> &data);

        protected:
            boost::asio::io_context _context; // The main context
            boost::asio::ip::udp::socket _socket; // The socket which will be used to send and receive the actions
            boost::asio::ip::udp::endpoint _endpoint; // endpoint of the Server

            // The buffer used to receive the actions
            std::array<char, MAX_PACKET_SIZE> _receiveBuffer; // The buffer used to receive the actions
//            std::vector<char> _receiveBuffer; // The buffer used to receive the actions

            // list of packets' ids receives
            std::vector<n_id_t> _receivedPacketsIds; // A list of packets' ids receives
            std::list<std::pair<boost::asio::ip::udp::endpoint, struct packet_s>>  _packetsSent; // A list of packets' ids receives
                // Mutexes shared
            std::mutex _receivedPacketsIdsMutex; // Mutex for the received packets ids
            std::mutex _outputQueueMutex; // Mutex for the output queue
            std::mutex _packetsSentMutex; // Mutex for the packets sent

        private:
            n_id_t _packetId = 0; // The packet id

            std::mutex _inputQueueMutex; // Mutex for the input queue
            std::mutex _tickMutex; // Mutex for the tick
            std::condition_variable _tickConvVar; // Condition variable for the tick

            // Threads
            std::vector<std::thread> _pool; // The context threads pool
            std::thread _clockThread; // The clock thread

            // Endpoints
            boost::asio::ip::udp::endpoint _senderEndpoint; // The sender endpoint

            // Body handler vars
            std::list<std::pair<
                SenderData,
                const actionHandler&>
            > _actions; // A list of actions which will be handled by the second context
            std::list<
                std::pair<boost::asio::ip::udp::endpoint, struct packet_s>
            > _outputQueue; // A queue of actions which will be sent to the clients
    }; // class INitwork
} // namespace NitworkServer

