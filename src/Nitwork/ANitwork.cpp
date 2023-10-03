/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ANitwork
*/

#include "ANitwork.hpp"
#include <fstream>
#include <iostream>

namespace Nitwork {
    ANitwork::ANitwork() : _socket(_context), _packetId(0)
    {
    }

    bool ANitwork::start(int port, int threadNb, int tick, const std::string &ip)
    {
        try {
            startReceiveHandler();
            startInputHandler();
            startOutputHandler();
            if (!startNitworkConfig(port, ip)) {
                std::cerr << "Error: Nitwork config failed" << std::endl;
                return false;
            }
            if (!startNitworkThreads(threadNb, tick)) {
                std::cerr << "Error: Nitwork threads failed" << std::endl;
                return false;
            }
            std::cout << "Nitwork started on port " << port << std::endl;
        } catch (std::exception &e) {
            std::cerr << "Nitwork Error : " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    bool ANitwork::startContextThreads(int threadNb)
    {
        std::cout << "Starting context threads" << std::endl;
        for (int i = 0; i < threadNb; i++) {
            _pool.emplace_back([this]() {
                try {
                    _context.run();
                } catch (std::exception &e) {
                    std::cerr << "Error: " << e.what() << std::endl;
                }
            });
            if (!_pool.back().joinable()) {
                std::cerr << "Error: thread nb: " << i << " not joinable" << std::endl;
                return false;
            }
        }
        return true;
    }

    bool ANitwork::startClockThread(int tick)
    {
        std::cout << "Starting clock thread" << std::endl;
        _clockThread = std::thread([this, tick]() {
            try {
                while (true) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(tick));
                    std::unique_lock<std::mutex> lockTick(_tickMutex);
                    _tickConvVar.notify_one();
                }
            } catch (std::exception &e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        });
        if (!_clockThread.joinable()) {
            std::cerr << "Error: clock thread not joinable" << std::endl;
            return false;
        }
        return true;
    }

    bool ANitwork::startNitworkThreads(int threadNb, int tick)
    {
        if (!startContextThreads(threadNb)) {
            std::cerr << "Error: context threads failed" << std::endl;
            return false;
        }
        if (!startClockThread(tick)) {
            std::cerr << "Error: clock thread failed" << std::endl;
            return false;
        }
        return true;
    }

    void ANitwork::stop()
    {
        _context.stop();
        for (auto &thread : _pool)
            thread.join();
        _pool.clear();
        _clockThread.join();
    }

    void ANitwork::startReceiveHandler()
    {
        _receiveBuffer.fill(0);
        _socket.async_receive_from(
            boost::asio::buffer(_receiveBuffer),
            _senderEndpoint,
            boost::bind(
                &ANitwork::headerHandler,
                this,
                boost::asio::placeholders::bytes_transferred,
                boost::asio::placeholders::error));
    }

    void ANitwork::headerHandler(std::size_t bytes_received, const boost::system::error_code &error)
    {
        std::cout << "abc: " << bytes_received << std::endl;
        if (bytes_received < sizeof(struct header_s)) {
            std::cerr << "Error: header not received" << std::endl;
            startReceiveHandler();
            return;
        }
        if (error) {
            std::cerr << "Error: " << error.message() << std::endl;
            startReceiveHandler();
            return;
        }
        auto *header = reinterpret_cast<struct header_s *>(_receiveBuffer.data());
        std::cout << "packet id :" << header->id << std::endl;
        if (header->magick1 != HEADER_CODE1 || header->magick2 != HEADER_CODE2) {
            std::cerr << "Error: header magick not valid" << std::endl;
            startReceiveHandler();
            return;
        }
        if (header->nb_action > MAX_NB_ACTION || header->nb_action < 0 || isAlreadyReceived(header->id)) {
            std::cerr << "Error: too many actions received or no action or already received" << std::endl;
            startReceiveHandler();
            return;
        }
        _receivedPacketsIds.push_back(header->id);
        //            handlePacketIdsReceived(*header);
        //        for (int i = 0; i < header->nb_action; i++) {
        handleBodyAction(_senderEndpoint);
        //        }
        startReceiveHandler();
    }

    bool ANitwork::isAlreadyReceived(n_id_t id)
    {
        for (auto &receivedId : _receivedPacketsIds) {
            if (receivedId == id)
                return true;
        }
        return false;
    }

    void ANitwork::handlePacketIdsReceived(const struct header_s &header)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        std::vector<int> ids;
        struct header_s *headerPacket;
        int index = 0;

        for (int i = 0; i < MAX_NB_ACTION; i++) {
            if (header.ids_received & (1 << i)) {
                ids.emplace_back(header.last_id_received - i);
            }
        }
        for (auto &id : ids) {
            auto it = std::find(_receivedPacketsIds.begin(), _receivedPacketsIds.end(), id);
            if (it == _receivedPacketsIds.end()) {
                for (auto &packet : _packetsSent) {
                    headerPacket = reinterpret_cast<struct header_s *>(
                        std::any_cast<struct header_s *>(packet.second.body));
                    if (headerPacket->id == id) {
                        _outputQueue.emplace_back(std::make_pair(packet.first, packet.second));
                        break;
                    }
                }
            }
            index++;
        }
    }

    void ANitwork::startInputHandler()
    {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockTick(_tickMutex, std::defer_lock);
            std::unique_lock<std::mutex> lockQueue(_inputQueueMutex, std::defer_lock);

            std::cout << std::endl << "Starting input handler" << std::endl;
            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    lockQueue.lock();
                    for (auto &action : _actions) {
                        action.second(action.first.data, action.first.endpoint);
                    }
                    _actions.clear();
                    lockQueue.unlock();
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void ANitwork::startOutputHandler()
    {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockQueue(_outputQueueMutex, std::defer_lock);
            std::unique_lock<std::mutex> lockTick(_tickMutex, std::defer_lock);
            const std::map<enum n_actionType_t, actionHandler> &actionToSendHandlers =
                getActionToSendHandlers();

            std::cout << std::endl << "Starting output handler" << std::endl;
            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    lockQueue.lock();
                    for (auto &data : _outputQueue) {
                        auto it = actionToSendHandlers.find(data.second.action);
                        if (it == actionToSendHandlers.end()) {
                            std::cerr << "Error: action not found" << std::endl;
                            continue;
                        }
                        addPacketToSentPackages(data);
                        it->second(data.second.body, data.first);
                    }
                    _outputQueue.clear();
                    lockQueue.unlock();
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void ANitwork::addPacketToSentPackages(
        const std::pair<boost::asio::ip::basic_endpoint<boost::asio::ip::udp>, packet_s> &data)
    {
        std::lock_guard<std::mutex> lock(_packetsSentMutex);

        _packetsSent.emplace_back(data);
        if (_packetsSent.size() > MAX_NB_ACTION) {
            _packetsSent.pop_front();
        }
    }

    /* Getters / Setters Section */
    n_id_t ANitwork::getPacketID()
    {
        n_id_t packetId = _packetId;
        _packetId++;
        return packetId;
    }

    void
    ANitwork::addPacketToSend(const boost::asio::ip::udp::endpoint &endpoint, const struct packet_s &packet)
    {
        std::lock_guard<std::mutex> lock(_outputQueueMutex);

        _outputQueue.emplace_back(std::make_pair(endpoint, packet));
    }
} // namespace Nitwork
