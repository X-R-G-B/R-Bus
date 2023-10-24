/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** MainNitwork
*/

#ifndef MAIN_NITWORK_H
    #define MAIN_NITWORK_H
    #include "Nitwork.h"

/* Message List Lobby */
PACK(struct connectionData_s {
    char ip[16];
    n_port_t port;
});

PACK(struct lobby_s {
    char name[32];
    n_id_t maxNbPlayer;
    struct connectionData_s lobbyInfos;
    struct connectionData_s ownerInfos;
});

PACK(struct msgNewLobby_s {
    n_magick_t magick;
    struct lobby_s lobby;
});

PACK(struct packetListLobby_s {
    struct header_s header;
    struct action_s action1;
    struct msgNewLobby_s firstLobby;
    struct action_s action2;
    struct msgNewLobby_s secondLobby;
    struct action_s action3;
    struct msgNewLobby_s thirdLobby;
    struct action_s action4;
    struct msgNewLobby_s fourthLobby;
    struct action_s action5;
    struct msgNewLobby_s fifthLobby;
});
#endif // MAIN_NITWORK_H
