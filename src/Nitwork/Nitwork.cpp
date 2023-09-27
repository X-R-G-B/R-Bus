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

    bool Nitwork::Start(int port) {
        try {
            _endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
            _socket.open(boost::asio::ip::udp::v4());
            _socket.bind(_endpoint);
            ContinuousReception();
            return true;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return false;
        }
    }

    bool Nitwork::Stop() {
        _context.stop();
        _inputThread.join();
        _clockThread.join();
        for (auto &thread : _outputThreads) {
            thread.join();
        }
    }

    void Nitwork::ContinuousReception() {
        _inputThread = std::thread([this]() {
            _context.run();
        });
    }

    void Nitwork::ClockThread(int tick) {
        _clockThread = std::thread([this]() {
            while (true) {
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
                _queueCondVar.notify_all();
            }
        });
    }

    void Nitwork::ClientsDatasHandler() {
        _outputThreads = std::array<std::thread, MAX_PLAYERS>();
        for (int i = 0; i < MAX_PLAYERS; i++) {
            _outputThreads.at(i) = std::thread([this]() {
                std::unique_lock<std::mutex> lock(_queueMutex);
                while (true) {
                    _queueCondVar.wait(lock);
                    // handle actions and send it to the client
                }
            });
        }
    }


    void Nitwork::handleBodyActionDatas(const struct action_s &action, const struct header_s &header, const boost::system::error_code& error, std::size_t bytes_received) {
        if (error) {
            std::cerr << "Error: " << error.message() << std::endl;
            return;
        }
        if (bytes_received != sizeof(enum n_actionType_t)) {
            std::cerr << "Error: body not received" << std::endl;
            return;
        }
        auto it = _actionsHandlers.find(action.magick);
        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            return;
        }
        it->second.first(header, it->second.second);
    }

    void Nitwork::handleBodyAction(const struct header_s &header) {
        struct action_s action = {NO_ACTION};

        _socket.async_receive_from(
            boost::asio::buffer(&action, sizeof(struct action_s)),
            _endpoint,
            boost::bind(
                &Nitwork::handleBodyActionDatas,
                this,
                action,
                header,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }

    void Nitwork::readDataFromEndpoint(boost::asio::ip::udp::endpoint &endpoint) {
        size_t header_bytes_received = 0;
        struct header_s header = {0, 0, 0, 0, 0, 0};

        while (true) {
            header_bytes_received = _socket.receive_from(boost::asio::buffer(&header, HEADER_SIZE), endpoint);

            if (header_bytes_received != sizeof(struct header_s)) {
                std::cerr << "Error: header not received" << std::endl;
                continue;
            }
            if (header.nb_action > MAX_NB_ACTION || header.nb_action < 0) {
                std::cerr << "Error: too many actions received or no action" << std::endl;
                continue;
            }
            for (int i = 0; i < header.nb_action; i++) {
                handleBodyAction(header);
            }
            break;
        }
    }

    void Nitwork::handleInitMsg(const std::any &msg) {
        const struct msgInit_s &initMsg = std::any_cast<struct msgInit_s>(msg);
        std::cout << "init" << std::endl;
    }

    void Nitwork::handleReadyMsg(const std::any &msg) {
        const struct msgReady_s &readyMsg = std::any_cast<struct msgReady_s>(msg);
        std::cout << "ready" << std::endl;
    }
}
