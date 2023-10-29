/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ANitwork
*/

#pragma once

#include <condition_variable>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <unordered_map>
#include "INitwork.hpp"
#include "Logger.hpp"
#include "Zstd.hpp"

namespace Nitwork {

    class ANitwork : public INitwork {
        public:
            ~ANitwork() override              = default;
            ANitwork(const ANitwork &)        = delete;
            ANitwork(const ANitwork &&)       = delete;
            void operator=(const ANitwork &)  = delete;
            void operator=(const ANitwork &&) = delete;

            void stop() override;

            bool isRunning() const final;

        protected:
            ANitwork();
            // start the NitworkServer
            bool start(int port, int threadNb, int tick, const std::string &ip = "") final;

            // send data to the endpoint with the given data
            template <typename T>
            void sendData(Packet &packet)
            {
                n_id_t id = getPacketId(packet.endpoint);

                if (packet.body.type() != typeid(T)) {
                    Logger::error("NITWORK: Invalid type");
                    return;
                }
                if constexpr (sizeof(T) > MAX_PACKET_SIZE) {
                    Logger::error("NITWORK: Package too big");
                    return;
                }
                T data = std::any_cast<T>(packet.body);
                if (!packet.getIsResend()) {
                    packet.id                 = id;
                    auto oldHeader            = static_cast<struct header_s>(data.header);
                    struct header_s newHeader = {
                        HEADER_CODE1,
                        getIdsReceived(packet.endpoint),
                        getLastIdsReceived(packet.endpoint),
                        id,
                        oldHeader.nb_action,
                        HEADER_CODE2};
                    data.header = newHeader;
                }
                std::shared_ptr<std::vector<char>> compressedPacket =
                    std::make_shared<std::vector<char>>(Zstd::compress(data));

                _socket.async_send_to(
                    boost::asio::buffer(*compressedPacket),
                    packet.endpoint,
                    [compressedPacket](const boost::system::error_code &error, std::size_t bytes_sent) {
                        Logger::debug("NITWORK: Package sent");
                        if (error) {
                            Logger::error("NITWORK: " + std::string(error.message()));
                            return;
                        }
                        if (bytes_sent != compressedPacket->size()) {
                            Logger::error("NITWORK: Package not sent");
                            return;
                        }
                    });
            }

            /* Getters / Setters */
            n_idsReceived_t getIdsReceived(const boost::asio::ip::udp::endpoint &endpoint);
            n_id_t getLastIdsReceived(const boost::asio::ip::udp::endpoint &endpoint);
            n_id_t getPacketId(const boost::asio::ip::udp::endpoint &endpoint);
            void addPacketToSend(const Packet &);
            void handlePacketIdsReceived(const struct header_s &header);

            void startReceiveHandler() final;
            // handler func for receive handler which handle the header
            template <typename B>
            void handleBody(const actionHandler &handler, const struct header_s &header)
            {
                // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
                auto *body = reinterpret_cast<B *>(_receiveBuffer.data());
                // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
                handleBodyDatas<B>(handler, header, *body, boost::system::error_code());
                std::memmove(
                    _receiveBuffer.data(),
                    _receiveBuffer.data() + sizeof(B),
                    _receiveBuffer.size() - sizeof(B));
                std::memset(_receiveBuffer.data() + _receiveBuffer.size() - sizeof(B), 0, sizeof(B));
            }

            // send package to all clients but not the endpoint
            void sendToAllClientsButNotOne(const Packet &packet, boost::asio::ip::udp::endpoint &endpoint);
            // send package to all clients
            void sendToAllClients(const Packet &packet);
            // check if the client is already connected
            bool isClientAlreadyConnected(boost::asio::ip::udp::endpoint &endpoint) const;

        private:
            // start the NitworkServer threads (context threads, clock thread, input thread and output
            // thread)
            bool startNitworkThreads(int threadNb, int tick) final;
            // start the context threads
            bool startContextThreads(int threadNb) final;
            // start the clock thread
            bool startClockThread(int tick) final;
            // start the input thread inside the context (post)
            void startInputHandler() final;
            // start the output thread inside the context (post)
            void startOutputHandler() final;
            // send package
            void sendPackages(const std::map<enum n_actionType_t, actionSender> &);
            // start receive handler
            void headerHandler(std::size_t bytes_received, const boost::system::error_code &error) final;
            // check if the packet has already been received
            bool isAlreadyReceived(n_id_t id, const boost::asio::ip::udp::endpoint &endpoint);
            // call startReceiveHandler method by displaying a message
            void callReceiveHandler(const std::string &message);

            // handler func for receive handler which handle the action
            template <typename B>
            void handleBodyDatas(
                const actionHandler &handler,
                const struct header_s &header,
                B &body,
                const boost::system::error_code &error)
            {
                if (error) {
                    Logger::error("NITWORK: " + std::string(error.message()));
                    startReceiveHandler();
                    return;
                }
                std::lock_guard<std::mutex> lock(_inputQueueMutex);
                SenderData senderData(header.id, _senderEndpoint, std::any(body));
                _actions.emplace_back(senderData, handler);
            }

            void addPacketToSentPackages(Packet &data);

        protected:
            // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
            boost::asio::io_context _context; // The main context
            boost::asio::ip::udp::socket
                _socket; // The socket which will be used to send and receive the actions
            std::list<boost::asio::ip::udp::endpoint> _endpoints;

            // The buffer used to receive the actions
            std::array<char, MAX_PACKET_SIZE> _receiveBuffer = {
                0}; // The buffer used to receive the actions
            //            std::vector<char> _receiveBuffer; // The buffer used to receive the actions

            // list of packets' ids receives
            // std::vector<n_id_t> _receivedPacketsIds; // A list of packets' ids receives
            std::unordered_map<boost::asio::ip::udp::endpoint, std::vector<n_id_t>> _receivedPacketsIdsMap;
            std::unordered_map<boost::asio::ip::udp::endpoint, n_id_t> _packetsIds;
            std::unordered_map<boost::asio::ip::udp::endpoint, std::list<Packet>>
                _packetsSent;                    // A list of packets' ids receives
                                                 // Mutexes shared
            std::mutex _receivedPacketsIdsMutex; // Mutex for the received packets ids
            std::mutex _outputQueueMutex;        // Mutex for the output queue
            std::mutex _packetsSentMutex;        // Mutex for the packets sent
            // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

        private:
            bool _isRunning = false; // A boolean to know if the NitworkServer is running

            std::mutex _inputQueueMutex;          // Mutex for the input queue
            std::mutex _tickMutex;                // Mutex for the tick
            std::condition_variable _tickConvVar; // Condition variable for the tick

            // Threads
            std::vector<std::thread> _pool; // The context threads pool
            std::thread _clockThread;       // The clock thread

            // Endpoints
            boost::asio::ip::udp::endpoint _senderEndpoint; // The sender endpoint

            // Body handler vars
            std::list<std::pair<
                SenderData,
                const actionHandler &>>
                _actions;                   // A list of actions which will be handled by the second context
            std::list<Packet> _outputQueue; // A queue of actions which will be sent to the clients
    };                                      // class INitwork
} // namespace Nitwork
