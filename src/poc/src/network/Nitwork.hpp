/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Nitwork library, a portable network library
*/

#pragma once

#include <list>
#include <condition_variable>
#include <mutex>
#include <asio.hpp>

#define MAX_ACTIONS 16
#define MAX_PLAYERS 4
#define HEADER_SIZE sizeof(int) * 4

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

            struct actionHeader_s {
                    actionType_t type;
            } __attribute__((packed));
            typedef char magick_t;

            typedef char anyAction;

            // Init action structs
            struct bodyInit_s {
                    struct actionHeader_s header;
            } __attribute__((packed));

            typedef char anyAction;

            struct msgInit_s {
                    struct header_s header;
                    bodyInit_s body; // mais en fait c’est any_action[]
            } __attribute__((packed));

            // Ready action structs
            struct bodyReady_s {
                    struct actionHeader_s header;
            } __attribute__((packed));

            struct msgReady_s {
                    struct header_s header;
                    struct bodyReady_s body; // mais en fait c’est any_action[]
            } __attribute__((packed));

            struct maxStruct_s {
                    int one;
                    int two;
                    int three;
                    int four;
                    int five;
                    int six;
                    int seven;
                    int eight;
                    int nine;
                    int ten;
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
                        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
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

//            template<typename T>
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
                        handleBody(header);
                    }
                    break;
                }
            }

            template<typename T>
            void sendDatasToAll(T &datas) {
                for (auto &endpoint : _endpoints) {
                    sendDatasToEndpoint(endpoint, datas);
                }
            }
            template<typename R, typename B>
            void handleBody(const struct Actions::header_s &header, const std::function<void(std::any)> &handler) {
                B body;
                size_t body_bytes_received;

                _socket.async_receive_from(asio::buffer(&body, sizeof(B)), _endpoint, [this, header, handler, body_bytes_received, body](const asio::error_code &error, size_t bytes_received) {
                  R resultData;
                  if (body_bytes_received != sizeof(B)) {
                      std::cerr << "Error: body not received" << std::endl;
                      return;
                  }
                  resultData.header = header;
                  resultData.body = body;
                  _actions.emplace_back(resultData);
                  _actionsHandler.emplace_back(handler);
                });
            }
            void handleBodyAction(const struct Actions::header_s &header) {
                size_t body_bytes_received;
                struct Actions::actionHeader_s action;

                body_bytes_received = _socket.receive_from(asio::buffer(&action, sizeof(Actions::actionHeader_s)), _endpoint);
                if (body_bytes_received != sizeof(Actions::actionHeader_s)) {
                    std::cerr << "Error: body not received" << std::endl;
                    return;
                }

//                switch (action.type) {
//                    case '~':
//                        struct Actions::msgInit_s init;
//                        struct Actions::bodyInit_s initBody;
//
//                        body_bytes_received = _socket.receive_from(asio::buffer(&initBody, sizeof(Actions::bodyInit_s)), _endpoint);
//                        if (body_bytes_received != sizeof(Actions::bodyInit_s)) {
//                            std::cerr << "Error: body not received" << std::endl;
//                            return;
//                        }
//                        init.header = header;
//                        init.body = initBody;
//                        _actions.push_back(init);
//                        _actionsHandler.push_back([](const std::any &action) {
//                            // handle init action
//                        });
//                        break;
//                    case '+':
//                        struct Actions::msgReady_s ready;
//                        struct Actions::bodyReady_s readyBody;
//
//                        body_bytes_received = _socket.receive_from(asio::buffer(&readyBody, sizeof(Actions::bodyReady_s)), _endpoint);
//                        if (body_bytes_received != sizeof(Actions::bodyReady_s)) {
//                            std::cerr << "Error: body not received" << std::endl;
//                            return;
//                        }
//                        ready.header = header;
//                        ready.body = readyBody;
//                        _actions.push_back(ready);
//                        _actionsHandler.push_back([](const std::any &action) {
//                            // handle ready action
//                        });
//                        break;
//                    default:
//                        break;
//                }
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
    };
}
