/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Internal header for NitworkServer C
*/

#ifndef NITWORK_H
    #define NITWORK_H

    #define HEADER_SIZE sizeof(struct header_s)
    #define TICKS_PER_SECOND 20
    #define ONE_SECOND 1000
    #define DEFAULT_THREAD_NB 4
    #define MAX_NB_ACTION 16
    #define MAX_CLIENTS 4
    #define HEADER_CODE1 '\x01'
    #define HEADER_CODE2 '\x03'


typedef char n_magick_t;
typedef int n_idsReceived_t;
typedef int n_id_t;
typedef int n_nbAction_t;

enum n_actionType_t {
    NO_ACTION = 0,
    INIT = 1,
    READY = 2,
    N_ACTION_TYPE_MAX,
};

struct header_s {
        n_magick_t magick1;
        n_idsReceived_t ids_received;
        n_id_t last_id_received;
        n_id_t id;
        n_nbAction_t nb_action;
        n_magick_t magick2;
} __attribute__((packed));

struct action_s {
        enum n_actionType_t magick;
} __attribute__((packed));

struct msgInit_s {
        n_magick_t magick;
} __attribute__((packed));

struct msgReady_s {
        n_magick_t magick;
} __attribute__((packed));

#endif
