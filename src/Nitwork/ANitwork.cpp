/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ANitwork
*/

#include "ANitwork.hpp"
#include <algorithm>
#include <thread>

namespace Nitwork {
    ANitwork::ANitwork() : _socket(_context), _packetId(0)
    {
    }

    bool ANitwork::start(int port, int threadNb, int tick, const std::string &ip)
    {
        try {
            _isRunning = true;
            if (!startNitworkConfig(port, ip)) {
                Logger::fatal("NITWORK: Nitwork config failed");
                return false;
            }
            startInputHandler();
            startOutputHandler();
            if (!startNitworkThreads(threadNb, tick)) {
                Logger::fatal("NITWORK: Nitwork threads failed");
                return false;
            }
            startReceiveHandler();
            Logger::info("NITWORK: Nitwork started on port " + std::to_string(port));
        } catch (std::exception &e) {
            Logger::fatal("NITWORK: Nitwork failed to start" + std::string(e.what()));
            return false;
        }
        return true;
    }

    bool ANitwork::startContextThreads(int threadNb)
    {
        for (int i = 0; i < threadNb; i++) {
            _pool.emplace_back([this]() {
                try {
                    _context.run();
                } catch (std::exception &e) {
                    Logger::fatal("NITWORK: " + std::string(e.what()));
                }
            });
            if (!_pool.back().joinable()) {
                Logger::fatal("NITWORK: Thread nb: " + std::to_string(i) + " not joinable");
                return false;
            }
        }
        return true;
    }

    bool ANitwork::startClockThread(int tick)
    {
        _clockThread = std::thread([this, tick]() {
            try {
                while (_isRunning) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(tick));
                    _tickMutex.lock();
                    _tickConvVar.notify_all();
                    _tickMutex.unlock();
                }
            } catch (std::exception &e) {
                Logger::fatal("NITWORK: catch clock thread: " + std::string(e.what()));
            }
        });
        if (!_clockThread.joinable()) {
            Logger::fatal("NITWORK: catch clock thread: not joinable");
            return false;
        }
        return true;
    }

    bool ANitwork::startNitworkThreads(int threadNb, int tick)
    {
        if (!startContextThreads(threadNb)) {
            Logger::fatal("NITWORK: context threads failed");
            return false;
        }
        if (!startClockThread(tick)) {
            Logger::fatal("NITWORK: clock thread failed");
            return false;
        }
        return true;
    }

    void ANitwork::stop()
    {
        _isRunning = false;
        _context.stop();
        for (auto &thread : _pool) {
            _tickMutex.lock();
            _tickConvVar.notify_all();
            _tickMutex.unlock();
            if (thread.joinable()) {
                thread.join();
            }
        }
        _clockThread.join();
        _pool.clear();
    }

    bool ANitwork::isRunning() const
    {
        return _isRunning;
    }

    void ANitwork::startReceiveHandler()
    {
        if (!_isRunning) {
            return;
        }
        _receiveBuffer.fill(0);
        _socket.async_receive_from(
            boost::asio::buffer(_receiveBuffer),
            _senderEndpoint,
            [this](const boost::system::error_code &error, std::size_t bytes_received) {
                headerHandler(bytes_received, error);
            });
    }

    void ANitwork::callReceiveHandler(const std::string &message)
    {
        //Logger::error("NITWORK: " + message);
        startReceiveHandler();
    }

    void ANitwork::headerHandler(std::size_t bytes_received, const boost::system::error_code &error)
    {
        if (error) {
            callReceiveHandler(error.message());
            return;
        }
        if (bytes_received < sizeof(struct header_s)) {
            callReceiveHandler("header not received");
            return;
        }
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        auto *header = reinterpret_cast<struct header_s *>(_receiveBuffer.data());
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast)

        if (header->magick1 != HEADER_CODE1 || header->magick2 != HEADER_CODE2) {
            callReceiveHandler("header magick not valid");
            return;
        }
        if (header->nb_action > MAX_NB_ACTION || isAlreadyReceived(header->id, _senderEndpoint)) {
            callReceiveHandler("header nb action not valid or already received");
            return;
        }
        _receivedPacketsIdsMutex.lock();
        _receivedPacketsIdsMap[_senderEndpoint].push_back(header->id);
        _receivedPacketsIdsMutex.unlock();
        handlePacketIdsReceived(*header);
        for (int i = 0; i < header->nb_action; i++) {
            handleBodyAction(*header, _senderEndpoint);
        }
        startReceiveHandler();
    }

    bool ANitwork::isAlreadyReceived(n_id_t id, const boost::asio::ip::udp::endpoint &endpoint)
    {
        return std::any_of(
            _receivedPacketsIdsMap[endpoint].begin(),
            _receivedPacketsIdsMap[endpoint].end(),
            [id](auto &receivedId) {
                return receivedId == id;
            });
    }

    void ANitwork::handlePacketIdsReceived(const struct header_s &header)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        std::vector<int> ids;
        std::list<Packet> &packetSent = _packetsSent[_senderEndpoint];

        for (int i = 0; i < MAX_NB_ACTION; i++) {
            ids.push_back((header.ids_received >> i) & 1);
        }
        std::reverse(ids.begin(), ids.end());
        for (std::size_t index = 0; index < packetSent.size(); index++) {
            if (ids[index] == 0) {
                auto packet =
                    std::find_if(packetSent.begin(), packetSent.end(), [header, index](auto &packet) {
                        return packet.id == header.last_id_received - index;
                    });
                if (packet == packetSent.end()) {
                    Logger::error(
                        "NITWORK: packet not found: " + std::to_string(header.last_id_received - index));
                    continue;
                }
                packet->setIsResend(true);
                addPacketToSend(*packet);
            }
        }
    }

    void ANitwork::startInputHandler()
    {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockTick(_tickMutex);

            try {
                while (_isRunning) {
                    _tickConvVar.wait(lockTick);
                    _inputQueueMutex.lock();
                    _actions.sort([](auto &a, auto &b) {
                        return a.first.id < b.first.id;
                    });
                    for (auto &action : _actions) {
                        try {
                           action.second(action.first.data, action.first.endpoint);
                        } catch (std::exception &e) {
                            Logger::error("NITWORK: catch action: " + std::string(e.what()));
                        }
                    }
                    _actions.clear();
                    _inputQueueMutex.unlock();
                }
            } catch (std::exception &e) {
                _inputQueueMutex.unlock();
                _isRunning = false;
                Logger::fatal("NITWORK: catch input thread: " + std::string(e.what()));
            }
        });
    }

    void ANitwork::sendPackages(const std::map<enum n_actionType_t, actionSender> &actionToSendHandlers)
    {
        for (auto &data : _outputQueue) {
            auto it = actionToSendHandlers.find(data.action);
            if (it == actionToSendHandlers.end()) {
                Logger::error("NITWORK: action not found");
                continue;
            }
            addPacketToSentPackages(data);
            try {
                it->second(data);
            } catch (std::exception &e) {
                Logger::error("NITWORK: catch action: " + std::string(e.what()));
            }
        }
    }

    void ANitwork::startOutputHandler()
    {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockTick(_tickMutex);
            const std::map<enum n_actionType_t, actionSender> &actionToSendHandlers =
                getActionToSendHandlers();

            try {
                while (_isRunning) {
                    _tickConvVar.wait(lockTick);
                    _outputQueueMutex.lock();
                    _outputQueue.sort([](auto &a, auto &b) {
                        return a.id < b.id;
                    });
                    sendPackages(actionToSendHandlers);
                    _outputQueue.clear();
                    _outputQueueMutex.unlock();
                }
            } catch (std::exception &e) {
                _outputQueueMutex.unlock();
                _isRunning = false;
                Logger::fatal("NITWORK: catch output thread: " + std::string(e.what()));
            }
        });
    }

    void ANitwork::addPacketToSentPackages(Packet &data)
    {
        std::lock_guard<std::mutex> lock(_packetsSentMutex);
        auto &endpointPacketsList = _packetsSent[data.endpoint];

        if (std::any_of(endpointPacketsList.begin(), endpointPacketsList.end(), [data](auto &packet) {
                return packet.id == data.id;
            })) {
            return;
        }
        endpointPacketsList.emplace_back(data);
        if (endpointPacketsList.size() > MAX_NB_ACTION) {
            endpointPacketsList.pop_front();
        }
    }

    /* Getters / Setters Section */
    n_id_t ANitwork::getPacketId(const boost::asio::ip::udp::endpoint &endpoint)
    {
        n_id_t packetId = _packetsIds[endpoint];
        _packetsIds[endpoint]++;
        return packetId;
    }

    /* Getters Section */
    n_idsReceived_t ANitwork::getIdsReceived(const boost::asio::ip::udp::endpoint &endpoint)
    {
        n_idsReceived_t idsReceived = 0;
        n_id_t lastId               = getLastIdsReceived(endpoint);
        bool isPresent              = false;

        if (_receivedPacketsIdsMap[endpoint].empty()) {
            return 0;
        }
        for (int i = 0; i < MAX_NB_ACTION; i++) {
            isPresent   = isAlreadyReceived(lastId - i, endpoint);
            idsReceived = idsReceived << 1;
            idsReceived += (isPresent ? 1 : 0);
        }
        return idsReceived;
    }

    n_id_t ANitwork::getLastIdsReceived(const boost::asio::ip::udp::endpoint &endpoint)
    {
        n_id_t lastId = 0;

        if (_receivedPacketsIdsMap[endpoint].empty()) {
            return 0;
        }
        std::sort(
            _receivedPacketsIdsMap[endpoint].begin(),
            _receivedPacketsIdsMap[endpoint].end(),
            [](auto &a, auto &b) {
                return a < b;
            });
        lastId = _receivedPacketsIdsMap[endpoint].back();
        return lastId;
    }

    void ANitwork::addPacketToSend(const Packet &packet)
    {
        std::lock_guard<std::mutex> lock(_outputQueueMutex);

        _outputQueue.emplace_back(packet);
    }
} // namespace Nitwork
