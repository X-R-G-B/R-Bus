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
#include <mutex>
#include "INitwork.hpp"
#include "Logger.hpp"

namespace Nitwork {
    constexpr int MAX_PACKET_SIZE = 1024;

    class ANitwork : public INitwork {
        public:
            ~ANitwork() override              = default;
            ANitwork(const ANitwork &)        = delete;
            ANitwork(const ANitwork &&)       = delete;
            void operator=(const ANitwork &)  = delete;
            void operator=(const ANitwork &&) = delete;

            // start the NitworkServer
            bool start(int port, int threadNb, int tick, const std::string &ip = "") override;

            void stop() override;
            // send data to the endpoint with the given data
            template <typename T>
            void sendData(std::any &rawData, boost::asio::ip::udp::endpoint &endpoint)
            {
                if (rawData.type() != typeid(T)) {
                    Logger::error("NITWORK: Invalid type");
                    return;
                }
                if constexpr (sizeof(T) > MAX_PACKET_SIZE) {
                    Logger::error("NITWORK: Package too big");
                    return;
                }
                T data = std::any_cast<T>(rawData);

                _socket.async_send_to(
                    boost::asio::buffer(&data, sizeof(T)),
                    endpoint,
                    [](const boost::system::error_code &error, std::size_t bytes_sent) {
                        Logger::info("NITWORK: Package sent");
                        if (error) {
                            Logger::error("NITWORK: " + std::string(error.message()));
                            return;
                        }
                        if (bytes_sent != sizeof(T)) {
                            Logger::error("NITWORK: Package not sent");
                            return;
                        }
                    });
            }

        protected:
            ANitwork();

            /* Getters / Setters */
            n_idsReceived_t getIdsReceived();
            n_id_t getPacketID();
            void addPacketToSend(const boost::asio::ip::udp::endpoint &, const Packet &);
            void handlePacketIdsReceived(const struct header_s &header);

            void startReceiveHandler() final;
            // handler func for receive handler which handle the header
            template <typename B>
            void handleBody(const actionHandler &handler, const struct header_s &header)
            {
                // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
                auto *body = reinterpret_cast<B *>(
                    _receiveBuffer.data() + sizeof(struct header_s) + sizeof(struct action_s));
                // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)
                handleBodyDatas<B>(handler, header, *body, boost::system::error_code());
            }

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
            void sendPackages(const std::map<enum n_actionType_t, actionHandler> &);
            // start receive handler
            void headerHandler(std::size_t bytes_received, const boost::system::error_code &error) final;
            // check if the packet has already been received
            bool isAlreadyReceived(n_id_t id);
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

            template <typename T>
            Packet updateHeaderPacket(const Packet &packet)
            {
                T data = std::any_cast<T>(packet.body);

                data.header.ids_received = getIdsReceived();
                auto updatedPacket       = Packet(packet.id, packet.action, std::make_any<T>(data));
                std::cout << "updatedPacket.header.ids_received: " << data.header.ids_received << std::endl;
                return updatedPacket;
            }

            void addPacketToSentPackages(
                const std::pair<boost::asio::ip::basic_endpoint<boost::asio::ip::udp>, Packet> &data);

        protected:
            // NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
            boost::asio::io_context _context; // The main context
            boost::asio::ip::udp::socket
                _socket; // The socket which will be used to send and receive the actions
            boost::asio::ip::udp::endpoint _endpoint; // endpoint of the Server

            // The buffer used to receive the actions
            std::array<char, MAX_PACKET_SIZE> _receiveBuffer = {
                0}; // The buffer used to receive the actions
            //            std::vector<char> _receiveBuffer; // The buffer used to receive the actions

            // list of packets' ids receives
            std::vector<n_id_t> _receivedPacketsIds; // A list of packets' ids receives
            std::list<std::pair<boost::asio::ip::udp::endpoint, Packet>>
                _packetsSent;                    // A list of packets' ids receives
                                                 // Mutexes shared
            std::mutex _receivedPacketsIdsMutex; // Mutex for the received packets ids
            std::mutex _outputQueueMutex;        // Mutex for the output queue
            std::mutex _packetsSentMutex;        // Mutex for the packets sent
            // NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)

        private:
            bool _isRunning = false; // A boolean to know if the NitworkServer is running

            n_id_t _packetId; // The packet id

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
                _actions; // A list of actions which will be handled by the second context
            std::list<std::pair<boost::asio::ip::udp::endpoint, Packet>>
                _outputQueue; // A queue of actions which will be sent to the clients
            std::map<enum n_actionType_t, std::function<Packet(const std::any &)>> _updatePacketHandlers = {
                {INIT,
                 [this](const std::any &any) {
                     return updateHeaderPacket<struct packetMsgInit_s>(std::any_cast<Packet>(any));
                 }},
                {READY,
                 [this](const std::any &any) {
                     return updateHeaderPacket<struct packetMsgReady_s>(std::any_cast<Packet>(any));
                 }},
                {START_GAME,
                 [this](const std::any &any) {
                     return updateHeaderPacket<struct packetMsgStartGame_s>(std::any_cast<Packet>(any));
                 }},
            }; // A map of actions which will be sent to the clients
    };         // class INitwork
} // namespace Nitwork
