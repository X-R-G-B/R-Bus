/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Nitwork library, a portable network library
*/

#pragma once

#include <list>
#include <boost/asio.hpp>

#define MAX_ACTIONS 16

namespace Nitwork {
    namespace Actions {
        extern "C"
        {
            // Parent structs
            typedef int idsReceived_t;
            typedef int id_t;
            typedef int nbAction_t;

            struct header_s {
                    idsReceived_t ids_received,
                    id_t last_id_received,
                    id_t id,
                    nbAction_t nb_action,
            } __attribute__((packed));

            typedef char actionType_t;

            struct actionHeader_s {
                    actionType_t type;
            } __attribute__((packed));
            typedef char magick_t;

            typedef char anyAction;

            // Init action structs
            struct bodyInit_s {
                    struct actionHeader_s header,
            } __attribute__((packed));

            typedef char anyAction;

            struct msgInit_s {
                    struct header_s header,
                    bodyInit_s body; // mais en fait c’est any_action[]
            } __attribute__((packed));

            // Ready action structs
            struct bodyReady_s {
                    struct action_header_s header,
            } __attribute__((packed));

            struct msgReady_s {
                    struct header_s header,
                    struct bodyReady_s body; // mais en fait c’est any_action[]
            } __attribute__((packed));

            struct maxStruct_s {
                    int one,
                    int two,
                    int three,
                    int four,
                    int five,
                    int six,
                    int seven,
                    int eight,
                    int nine,
                    int ten,
            } __attribute__((packed));
        }
    }
    class Nitwork {
        public:
            Nitwork() = default;
            ~Nitwork() = delete;

            Nitwork &getInstance() {
                static Nitwork instance;

                return instance;
            }
            sendInit() {

            }
            sendReady() {

            }
        protected:
        private:
            asio::io_context io_context;
            std::list<Actions::maxStruct_s> actions;
            std::array<Actions::id_t, MAX_ACTIONS> ids;
    };
}
