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
                    idsReceived_t ids_received;
                    id_t last_id_received;
                    id_t id;
                    nbAction_t nb_action;
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
            Nitwork(const std::string &ip, int port)
                : _input_context(), _output_context(), _socket(_input_context), _endpoint() {
                initConnection(ip, port);
            }
            Nitwork()
                : _input_context(),
                    _output_context(),
                  _socket(_input_context) {}

            Nitwork &getInstance() {
                static Nitwork instance;

                return instance;
            }

            void start() {
                _input_context.run();
            }

            // Initialise the connection with the server
//            bool initConnection(const std::string &ip, int port) {
//                try {
//                    _endpoint = asio::ip::udp::endpoint(asio::ip::address::from_string(ip), port);
//                    _socket.open(asio::ip::udp::v4());
//                    return true;
//                } catch (std::exception &e) {
//                    std::cerr << e.what() << std::endl;
//                    return false;
//                }
//            }

            // Loop to check if anything is received, and add it to the list of actions
//            void run() {
//            }
//            void readDatasFromEndpoint(asio::ip::udp::endpoint &endpoint) {
//                size_t header_bytes_received;
//                struct Actions::header_s header;
//
//                while (true) {
//                    header_bytes_received = _socket.receive_from(asio::buffer(&header, HEADER_SIZE), endpoint);
//
//                    if (header_bytes_received != sizeof(Actions::header_s)) {
//                        std::cerr << "Error: header not received" << std::endl;
//                        continue;
//                    }
//                    if (header.nb_action > MAX_ACTIONS || header.nb_action < 0) {
//                        std::cerr << "Error: too many actions received or no action" << std::endl;
//                        continue;
//                    }
//                    for (int i = 0; i < header.nb_action; i++) {
//                        handleBody(header);
//                        //                        _actions.push_back(action);
//                    }
//                }
//            }
//            void handleBody(const struct Actions::header_s &header) {
//                size_t body_bytes_received;
//                struct Actions::actionHeader_s action;
//
//                body_bytes_received = _socket.receive_from(asio::buffer(&action, sizeof(Actions::actionHeader_s)), _endpoint);
//
//                if (body_bytes_received != sizeof(Actions::actionHeader_s)) {
//                    std::cerr << "Error: body not received" << std::endl;
//                    return;
//                }
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
//                        break;
//                    default:
//                        break;
//                }
//            }
//            void addActions(Actions::maxStruct_s action) {
//                _actions.push_back(action);
//            }
//            void sendInit() {
//            }
//            void sendReady() {
//            }
        protected:
        private:
            asio::io_context _inputContext;
            asio::io_context _outputContext;
            std::thread _inputThread;
            std::thread _clockThread;
            std::array<std::thread, 4> _outputThreads;
            std::list<Actions::maxStruct_s> _actions;
            std::mutex _queueMutex;
            std::condition_variable _queueCondVar;


            asio::ip::udp::socket _socket;
            asio::ip::udp::endpoint _endpoint;
            std::array<Actions::id_t, MAX_ACTIONS> _ids;
    };
}
