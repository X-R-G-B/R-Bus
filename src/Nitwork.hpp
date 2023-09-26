/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Nitwork library, a portable network library
*/

#pragma once

#include <iostream>
#include <list>
#include <map>
#include <any>
#include <condition_variable>
#include <mutex>
#include <boost/bind/bind.hpp>
#include <boost/asio/placeholders.hpp>
#include <asio.hpp>

#define MAX_ACTIONS 16
#define MAX_PLAYERS 4
#define HEADER_SIZE sizeof(int) * 4
#define TICKS_PER_SECOND 128

namespace Nitwork {
    namespace Actions {
        extern "C"
        {
            // Parent structs
            typedef int idsReceived_t;
            typedef int id_t;
            typedef int nbAction_t;

            struct header_s {
                    char magick1;
                    idsReceived_t ids_received;
                    id_t last_id_received;
                    id_t id;
                    nbAction_t nb_action;
                    char magick2;
            } __attribute__((packed));

            typedef char actionType_t;

            struct action_s {
                    actionType_t magick;
            } __attribute__((packed));
            // Init action structs

            struct msgInit_s {
                    actionType_t magick; // type of the action
            } __attribute__((packed));

            struct msgReady_s {
                    actionType_t magick; // type of the action
            } __attribute__((packed));
        }
    }
    class Nitwork {
        public:
            Nitwork()
                : _context(),
                  _socket(_context) {}

            Nitwork &getInstance() {
                static Nitwork instance;

                return instance;
            }

            // start the server
            int Start(int port = 4242) {
                try {
                    _endpoint = asio::ip::udp::endpoint(asio::ip::udp::v4(), port);
                    _socket.open(asio::ip::udp::v4());
                    _socket.bind(_endpoint);
                    ContinuousReception();
                    return true;
                } catch (std::exception &e) {
                    std::cerr << e.what() << std::endl;
                    return false;
                }
            }
            int Stop() {
                _context.stop();
                _inputThread.join();
                _clockThread.join();
                for (auto &thread : _outputThreads) {
                    thread.join();
                }
            }
            void ContinuousReception() {
                _inputThread = std::thread([this]() {
                    _context.run();

                });
            }
            // Method which handle clock and unlock client threads each n ticks
            void ClockThread(int tick) {
                _clockThread = std::thread([this]() {
                    while (true) {
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / TICKS_PER_SECOND));
                        _queueCondVar.notify_all();
                    }
                });
            }
            void ClientsDatasHandler() {
                _outputThreads = std::array<std::thread, MAX_PLAYERS>();
                for (int i = 0; i < MAX_PLAYERS; i++) {
                    _outputThreads[i] = std::thread([this]() {
                        std::unique_lock<std::mutex> lock(_queueMutex);
                        while (true) {
                            _queueCondVar.wait(lock);
                            // handle actions and send it to the client
                        }
                    });
                }
            }
            template<typename T>
            void sendDatasToEndpoint(asio::ip::udp::endpoint &endpoint, T &datas) {
                _socket.send_to(asio::buffer(&datas, sizeof(T)), endpoint);
            }

            template<typename T>
            void sendDatasToAll(T &datas) {
                for (auto &endpoint : _endpoints) {
                    sendDatasToEndpoint(endpoint, datas);
                }
            }
            template<typename B>
            void handleBodyDatas(const struct Actions::header_s &header, const std::function<void(std::any)> &handler, B body, const asio::error_code& error, const std::size_t bytes_received) {
                if (error) {
                    std::cerr << "Error: " << error.message() << std::endl;
                    return;
                } else if (bytes_received != sizeof(B)) {
                    std::cerr << "Error: body not received" << std::endl;
                    return;
                }
                _actions.emplace_back(body);
                _actionsHandler.emplace_back(handler);
            }
            template<typename B>
            void handleBody(const Actions::header_s &header, const std::function<void(const std::any &)> &handler) {
                B body;

                _socket.async_receive_from(asio::buffer(&body, sizeof(B)), _endpoint, boost::bind(&Nitwork::handleBodyDatas<B>, this, header, handler, body, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }
            void handleBodyActionDatas(const Actions::action_s &action, const Actions::header_s &header, const asio::error_code& error, const std::size_t bytes_received) {
                if (error) {
                    std::cerr << "Error: " << error.message() << std::endl;
                    return;
                } else if (bytes_received != sizeof(Actions::actionType_t)) {
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
            void handleBodyAction(const Actions::header_s &header) {
                Actions::action_s action;

                _socket.async_receive_from(asio::buffer(&action, sizeof(Actions::action_s)), _endpoint, boost::bind(&Nitwork::handleBodyActionDatas, this, action, header, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
            }
            void readDataFromEndpoint(asio::ip::udp::endpoint &endpoint) {
                size_t header_bytes_received;
                struct Actions::header_s header;

                while (true) {
                    header_bytes_received = _socket.receive_from(asio::buffer(&header, HEADER_SIZE), endpoint);

                    if (header_bytes_received != sizeof(Actions::header_s)) {
                        std::cerr << "Error: header not received" << std::endl;
                        continue;
                    }
                    if (header.nb_action > MAX_ACTIONS || header.nb_action < 0) {
                        std::cerr << "Error: too many actions received or no action" << std::endl;
                        continue;
                    }
                    for (int i = 0; i < header.nb_action; i++) {
                        handleBodyAction(header);
                    }
                    break;
                }
            }
            void handleInitMsg(const std::any &msg) {
                const Actions::msgInit_s &initMsg = std::any_cast<Actions::msgInit_s>(msg);
                std::cout << "init" << std::endl;
            }
            void handleReadyMsg(const std::any &msg) {
                const Actions::msgReady_s &readyMsg = std::any_cast<Actions::msgReady_s>(msg);
                std::cout << "ready" << std::endl;
            }

        protected:
        private:
            asio::io_context _context; // second context which will handle the outputs (handle actions and send it, each n ticks
            std::thread _inputThread; // A thread for the first context
            std::thread _clockThread; // A thread for the clock which is in the second context
            std::array<std::thread, MAX_PLAYERS> _outputThreads; // A thread for each player which is in the second context

            // Body handler vars
            std::list<std::any> _actions; // A list of actions which will be handled by the second context
            std::list<std::function<void(const std::any &)>> _actionsHandler; // A list of functions which will be used to handle the actions

            std::mutex _queueMutex; // A mutex to lock the queue which will be used by both contexts
            std::condition_variable _queueCondVar; // A condition variable to wait for the queue to be used by the second context
            std::vector<asio::ip::udp::endpoint> _endpoints; // A vector of endpoints which will be used to send the actions to the clients and identify them
            std::array<Actions::id_t, MAX_ACTIONS> _ids; // An array of ids which will be used to identify the actions
            asio::ip::udp::socket _socket; // The socket which will be used to send and receive the actions
            asio::ip::udp::endpoint _endpoint; // The endpoint which will be used to send and receive the actions

            std::map<
                Actions::actionType_t,
                std::pair<
                    std::function<void(const Actions::header_s &, const std::function<void(const std::any &)> &)>,
                    std::function<void(const std::any &)>
                    >
                > _actionsHandlers = {
                    {'~',
                        std::make_pair(
                            std::function<void(const Actions::header_s &, const std::function<void(const std::any &)> &)>(std::bind(&Nitwork::handleBody<Actions::msgInit_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &)>(std::bind(&Nitwork::handleInitMsg, this, std::placeholders::_1))
                        )
                    },
                    {'#',
                        std::make_pair(
                            std::function<void(const Actions::header_s &, const std::function<void(const std::any &)> &)>(std::bind(&Nitwork::handleBody<Actions::msgReady_s>, this, std::placeholders::_1, std::placeholders::_2)),
                            std::function<void(const std::any &)>(std::bind(&Nitwork::handleReadyMsg, this, std::placeholders::_1))
                        )
                    }
                };

    };
}
