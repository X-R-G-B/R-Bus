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

    bool Nitwork::isRunning() const {
        return _isRunning;
    }

    bool Nitwork::start(int port) {
        try {
            _endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
            _socket.open(boost::asio::ip::udp::v4());
            _socket.bind(_endpoint);
            inputHandler();
            clockThread(TICKS_PER_SECOND);
            outputHandler();
            _isRunning = true;
            std::cout << "Server started" << std::endl;
            return true;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    void Nitwork::stop() {
        _inputThread.join();
        _clockThread.join();
        _outputThread.join();
        _isRunning = false;
        std::cout << "Server stopped" << std::endl;
    }

    void Nitwork::inputHandler() {
        _inputThread = std::thread([this]() {
            std::unique_lock<std::mutex> lock(_queueMutex);
            while (true) {
                _queueCondVar.wait(lock);
//                std::cout << "input handler" << std::endl;
                readDataFromEndpoint();
                for (auto &action : _actions) {
                    action.second(action.first.data, action.first.endpoint);
                }
            }
        });
    }

    void Nitwork::clockThread(int tick) {
        _clockThread = std::thread([this, tick]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / tick));
                _queueCondVar.notify_all();
            }
        });
    }

    void Nitwork::outputHandler() {
        _outputThread = std::thread([this]() {
            std::unique_lock<std::mutex> lock(_queueMutex);
            std::size_t size = 0;
            while (true) {
                _queueCondVar.wait(lock);
//                std::cout << "output handler" << std::endl;
                size = _outputQueue.size();
                for (std::size_t i = 0; i < size; i++) {
                    lock.unlock();
                    auto &data = _outputQueue.front();
                    _outputQueue.pop_front();
                    _actionToSendHandlers[data.second.action](data.first, data.second.body);
                    lock.lock();
                }
            }
        });
    }

    void Nitwork::handleBodyActionData(const struct action_s &action, const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint, const boost::system::error_code& error, std::size_t bytes_received) {
        if (error) {
            std::cerr << "Error: " << error.message() << std::endl;
            return;
        }
        if (bytes_received != sizeof(enum n_actionType_t)) {
            std::cerr << "Error: body not received" << std::endl;
            return;
        }
        auto endPointIt = std::find(_endpoints.begin(), _endpoints.end(), endpoint);
        if (endPointIt == _endpoints.end() && action.magick != INIT) {
            std::cerr << "Error: endpoint not found" << std::endl;
            return;
        }
        auto it = _actionsHandlers.find(action.magick);
        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            return;
        }
        it->second.first(header, it->second.second);
    }

    void Nitwork::handleBodyAction(const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint) {
        struct action_s action = {NO_ACTION};

        _socket.async_receive_from(
            boost::asio::buffer(&action, sizeof(struct action_s)),
            _endpoint,
            boost::bind(
                &Nitwork::handleBodyActionData,
                this,
                action,
                header,
                endpoint,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

    void Nitwork::readDataFromEndpointHandler(const struct header_s &header, const boost::asio::ip::udp::endpoint &endpoint, std::size_t bytes_received, const boost::system::error_code& error) {
        if (error) {
            std::cerr << "Error: " << error.message() << std::endl;
            return;
        }
        if (bytes_received != sizeof(struct header_s)) {
            std::cerr << "Error: header not received" << std::endl;
            return;
        }
        if (header.nb_action > MAX_NB_ACTION || header.nb_action < 0) {
            std::cerr << "Error: too many actions received or no action" << std::endl;
            return;
        }
        std::cout << "header received" << std::endl;
        for (int i = 0; i < header.nb_action; i++) {
            handleBodyAction(header, endpoint);
        }
    }

    void Nitwork::readDataFromEndpoint() {
        struct header_s header = {0, 0, 0, 0, 0, 0};
        boost::asio::ip::udp::endpoint endpoint;

        _socket.async_receive_from(boost::asio::buffer(&header, HEADER_SIZE), endpoint, boost::bind(&Nitwork::readDataFromEndpointHandler, this, header, endpoint, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
    }

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

        /// A SUPPRIMER
        stop();
    }

    void Nitwork::handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint) {
        const struct msgReady_s &readyMsg = std::any_cast<struct msgReady_s>(msg);
        std::cout << "ready" << std::endl;
    }
}
