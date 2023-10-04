/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** ANitwork
*/

#include <algorithm>
#include "ANitwork.hpp"

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
        for (auto &thread : _pool) {
            thread.join();
        }
        _pool.clear();
        _clockThread.join();
    }

    void ANitwork::startReceiveHandler()
    {
        _receiveBuffer.fill(0);
        _socket.async_receive_from(
            boost::asio::buffer(_receiveBuffer),
            _senderEndpoint,
            [this](const boost::system::error_code &error, std::size_t bytes_received) {
                headerHandler(bytes_received, error);
            }
        );
    }

    void ANitwork::headerHandler(std::size_t bytes_received, const boost::system::error_code &error)
    {
        std::unique_lock<std::mutex> lock(_receivedPacketsIdsMutex, std::defer_lock);

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
        // NOLINT BEGIN(cppcoreguidelines-pro-type-reinterpret-cast)
        auto *header = reinterpret_cast<struct header_s *>(_receiveBuffer.data());
        // NOLINT END(cppcoreguidelines-pro-type-reinterpret-cast)

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
        lock.lock();
        _receivedPacketsIds.push_back(header->id);
        lock.unlock();
        handlePacketIdsReceived(*header);
        for (int i = 0; i < header->nb_action; i++) {
            handleBodyAction(*header, _senderEndpoint);
        }
        startReceiveHandler();
    }

    bool ANitwork::isAlreadyReceived(n_id_t id)
    {
        return std::any_of(_receivedPacketsIds.begin(), _receivedPacketsIds.end(), [id](auto &receivedId) {
            return receivedId == id;
        });
    }

    void ANitwork::handlePacketIdsReceived(const struct header_s &header)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        std::vector<int> ids;

        for (int i = 0; i < MAX_NB_ACTION; i++) {
            ids.push_back((header.ids_received >> i) & 1);
        }
        std::reverse(ids.begin(), ids.end());
        for (std::size_t index = 0; index < _packetsSent.size(); index++) {
            if (!ids[index]) {
                auto packet = std::find_if(_packetsSent.begin(), _packetsSent.end(), [header, index](auto &packet) {
                    return std::size_t(packet.second.id) == header.last_id_received - index;
                });
                if (packet == _packetsSent.end()) {
                    std::cerr << "Error: packet not found" << std::endl;
                    continue;
                }
                auto newPacket = _updatePacketHandlers[packet->second.action](packet->second);
                addPacketToSend(packet->first, newPacket);
            }
        }
    }

    void ANitwork::startInputHandler()
    {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockTick(_tickMutex, std::defer_lock);
            std::unique_lock<std::mutex> lockQueue(_inputQueueMutex, std::defer_lock);

            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    lockQueue.lock();
                    _actions.sort([](auto &a, auto &b) {
                        return a.first.id < b.first.id;
                    });
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

            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    lockQueue.lock();
                    _outputQueue.sort([](auto &a, auto &b) {
                        return a.second.id < b.second.id;
                    });
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
        const std::pair<boost::asio::ip::basic_endpoint<boost::asio::ip::udp>, Packet> &data)
    {
        std::lock_guard<std::mutex> lock(_packetsSentMutex);

        if (std::any_of(_packetsSent.begin(), _packetsSent.end(), [data](auto &packet) {
                return packet.second.id == data.second.id;
            })) {
            return;
        }
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

    /* Getters Section */
    n_idsReceived_t ANitwork::getIdsReceived()
    {
        n_idsReceived_t idsReceived = 0;
        int lastId = 0;
        bool isPresent = false;

        if (_receivedPacketsIds.empty()) {
            return 0;
        }
        std::sort(_receivedPacketsIds.begin(), _receivedPacketsIds.end(), [](auto &a, auto &b) {
            return a < b;
        });
        lastId = _receivedPacketsIds.back();
        for (int i = 0; i < MAX_NB_ACTION; i++) {
            isPresent = std::any_of(_receivedPacketsIds.begin(), _receivedPacketsIds.end(), [lastId, i](auto &id) {
                return id == lastId - i;
            });
            idsReceived = idsReceived << 1;
            idsReceived += (isPresent ? 1 : 0);
        }
        return idsReceived;
    }

    void
    ANitwork::addPacketToSend(const boost::asio::ip::udp::endpoint &endpoint, const Packet &packet)
    {
        std::lock_guard<std::mutex> lock(_outputQueueMutex);

        _outputQueue.emplace_back(endpoint, packet);
    }
} // namespace Nitwork
