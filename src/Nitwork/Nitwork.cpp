/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Implementation of nitwork
*/

#include "Nitwork.hpp"

namespace Nitwork {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    Nitwork Nitwork::_instance = Nitwork();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    Nitwork &Nitwork::getInstance() {
        return _instance;
    }

    /* Start Section */
    bool Nitwork::start(int port, int threadNb, int tick) {
        try {
            startReceiveHandler();
            startInputHandler();
            startOutputHandler();
            if (!startServerConfig(port)) {
                std::cerr << "Error: server config failed" << std::endl;
                return false;
            }
            if (!startServerThreads(threadNb, tick)) {
                std::cerr << "Error: server threads failed" << std::endl;
                return false;
            }
            std::cout << "Server started on port " << port << " on " << boost::asio::ip::host_name() << " with ip " << _endpoint.address().to_string() << std::endl;
        } catch (std::exception &e) {
            std::cerr << "Nitwork Error : " << e.what() << std::endl;
            return false;
        }
        return true;
    }

    bool Nitwork::startServerConfig(int port)
    {
        _endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
        _socket.open(boost::asio::ip::udp::v4());
        if (!_socket.is_open()) {
            std::cerr << "Error: socket not open" << std::endl;
            return false;
        }
        _socket.bind(_endpoint);
        return true;
    }

    bool Nitwork::startClockThread(int tick) {
        _clockThread = std::thread([this, tick]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(ONE_SECOND / tick));
                _tickConvVar.notify_all();
            }
        });
        if (!_clockThread.joinable()) {
            std::cerr << "Error: clock thread not joinable" << std::endl;
            return false;
        }
        return true;
    }

    bool Nitwork::startServerThreads(int threadNb, int tick)
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

    bool Nitwork::startContextThreads(int threadNb) {
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

    void Nitwork::startInputHandler() {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockTick(_tickMutex);

            std::cout << std::endl << "Starting input handler" << std::endl;
            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    for (auto &action : _actions) {
                        action.second(action.first.data, action.first.endpoint);
                    }
                    _actions.clear();
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }

    void Nitwork::startOutputHandler() {
        boost::asio::post(_context, [this]() {
            std::unique_lock<std::mutex> lockQueue(_outputQueueMutex);
            std::unique_lock<std::mutex> lockTick(_tickMutex);

            std::cout << std::endl << "Starting output handler" << std::endl;
            try {
                while (true) {
                    _tickConvVar.wait(lockTick);
                    for (auto &data : _outputQueue) {
                        lockQueue.lock();
                        _actionToSendHandlers[data.second.action](data.first, data.second.body);
                        lockQueue.unlock();
                    }
                    _outputQueue.clear();
                }
            } catch (std::exception &e) {
                std::cerr << e.what() << std::endl;
            }
        });
    }
    /* End Start Section */

    /* Receive Section */
    void Nitwork::startReceiveHandler() {

        std::cout << "Starting receive handler" << std::endl;
        _socket.async_receive_from(
            boost::asio::buffer(_receiveBuffer),
            _clientEndpoint,
            boost::bind(
                &Nitwork::headerHandler,
                this,
                boost::asio::placeholders::bytes_transferred,
                boost::asio::placeholders::error
            )
        );
    }

    void Nitwork::headerHandler(std::size_t bytes_received, const boost::system::error_code& error) {
        if (error) {
            std::cerr << "Error: " << error.message() << std::endl;
            startReceiveHandler();
            return;
        }
        try {
            auto *header =
                reinterpret_cast<struct header_s *>(_receiveBuffer.data());
            if (bytes_received < sizeof(struct header_s)) {
                std::cerr << "Error: header not received" << std::endl;
                startReceiveHandler();
                return;
            }
            if (header->nb_action > MAX_NB_ACTION || header->nb_action < 0) {
                std::cerr << "Error: too many actions received or no action"
                          << std::endl;
                startReceiveHandler();
                return;
            }
            std::cout << "header received" << std::endl;
            for (int i = 0; i < header->nb_action; i++) {
                handleBodyAction(*header, _clientEndpoint);
            }
        } catch (std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    void Nitwork::handleBodyAction(const struct header_s header, const boost::asio::ip::udp::endpoint &endpoint) {
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data() + sizeof(struct header_s));
        std::cout << "action.magick: " << action->magick << std::endl;
        auto endPointIt = std::find(_endpoints.begin(), _endpoints.end(), endpoint);
        if (endPointIt == _endpoints.end() && action->magick != INIT) {
            std::cerr << "Error: endpoint not found" << std::endl;
            startReceiveHandler();
            return;
        }
        auto it = _actionsHandlers.find(action->magick);
        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            startReceiveHandler();
            return;
        }
        it->second.first(header, it->second.second);
        startReceiveHandler();
    }
    /* End Receive Section */

    /* Stop Section */
    void Nitwork::stop() {
        _inputThread.join();
        _clockThread.join();
        _outputThread.join();
        _context.stop();
        std::cout << "Server stopped" << std::endl;
    }
    /* End Stop Section */

    /* Handle packet (msg) Section */
    void Nitwork::handleInitMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
        const struct msgInit_s &initMsg = std::any_cast<struct msgInit_s>(msg);

        std::cout << "init" << std::endl;
        if (_endpoints.size() >= MAX_CLIENTS) {
            std::cerr << "Error: too many clients" << std::endl;
            return;
        }
        auto endPointIt = std::find(_endpoints.begin(), _endpoints.end(), endpoint);
        if (endPointIt != _endpoints.end()) {
            std::cerr << "Error: endpoint already init" << std::endl;
            return;
        }
        _endpoints.emplace_back(endpoint);

    }

    void Nitwork::handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
        const struct msgReady_s &readyMsg = std::any_cast<struct msgReady_s>(msg);
        std::cout << "ready" << std::endl;
    }
    /* End Handle packet (msg) Section */
}
