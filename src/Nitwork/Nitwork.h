/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Internal header for NitworkServer C
*/

#ifndef NITWORK_H
    #define NITWORK_H

    #ifdef __GNUC__
        #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
    #endif

    #ifdef _MSC_VER
        #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
    #endif

    #define HEADER_SIZE sizeof(struct header_s)
    #define TICKS_PER_SECOND 20
    #define ONE_SECOND 1000
    #define DEFAULT_THREAD_NB 4
    #define MAX_NB_ACTION 16
    #define MAX_CLIENTS 4
    #define HEADER_CODE1 '\x01'
    #define HEADER_CODE2 '\x03'

    #define MAGICK_INIT '\x06'
    #define MAGICK_READY '\x17'
    #define MAGICK_START_GAME '\a'


typedef char n_magick_t;
typedef int n_idsReceived_t;
typedef int n_id_t;
typedef int n_nbAction_t;

enum n_actionType_t {
    NO_ACTION = 0,
    INIT = 1,
    READY = 2,
    START_GAME = 3,
    N_ACTION_TYPE_MAX,
};

PACK(struct header_s {
        n_magick_t magick1;
        n_idsReceived_t ids_received;
        n_id_t last_id_received;
        n_id_t id;
        n_nbAction_t nb_action;
        n_magick_t magick2;
});

PACK(struct action_s {
        enum n_actionType_t magick;
});


/* Message Init */
PACK(struct msgInit_s {
        n_magick_t magick;
});

PACK(struct packetMsgInit_s {
        struct header_s header;
        struct action_s action;
        struct msgInit_s msgInit;
});


/* Message ready */
PACK(struct msgReady_s {
        n_magick_t magick;
});

PACK(struct packetMsgReady_s {
        struct header_s header;
        struct action_s action;
        struct msgReady_s msgReady;
});

/* Message Start Game */
PACK(struct msgStartGame_s {
        n_magick_t magick;
        n_id_t playerId;
});

PACK(struct packetMsgStartGame_s {
        struct header_s header;
        struct action_s action;
        struct msgStartGame_s msgStartGame;
});

#endif
