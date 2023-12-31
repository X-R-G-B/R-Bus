/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Internal header for NitworkServer C
*/


#ifndef NITWORK_H
    #define NITWORK_H

    #include "MessageTypes.h"

/* Configuration */
    #define HEADER_SIZE sizeof(struct header_s) // The size of the header
    #define TICKS 128 // The number of ticks per second
    #define TICKS_PER_SECOND(t) (t / TICKS) // The number of ticks per second
    #define TICKS_PER_MILLISECOND(t) (TICKS_PER_SECOND(t) / 1000) // The number of ticks per millisecond
    #define DEFAULT_THREAD_NB 4 // The default number of thread
    #define RANDOM_PORT 0 // The port to use to get a random port (the first available)
    #define MAX_MAIN_SERVER_CLIENT 300 // The max number of client for the main server
    #define MAX_NB_ACTION 16 // The max number of action per packet

/* The header code of the packet (used to check if the packet is valid) */
    #define HEADER_CODE1 '\x01'
    #define HEADER_CODE2 '\x03'

/* The magick number of the packet (used to check if the packet is valid) */
    #define MAGICK_INIT '\x06'
    #define MAGICK_READY '\x17'
    #define MAGICK_START_WAVE '\x07'
    #define MAGICK_POSITION_RELATIVE '\x08'
    #define MAGICK_POSITION_ABSOLUTE '\x09'
    #define MAGICK_LIFE_UPDATE '\x0b'
    #define MAGICK_ENEMY_DEATH '\x0c'
    #define MAGICK_NEW_ENEMY '\x0e'
    #define MAGICK_NEW_MISSILE '\x0d'
    #define MAGICK_POSITION_RELATIVE_BROADCAST '\x0f'
    #define MAGICK_POSITION_ABSOLUTE_BROADCAST '\x10'
    #define MAGICK_NEW_PLAYER '\x0a'
    #define MAGICK_PLAYER_DEATH '\x11'
    #define MAGICK_LIST_LOBBY '\x12'
    #define MAGICK_CREATE_LOBBY '\x13'
    #define MAGICK_REQUEST_LIST_LOBBY '\x16'
    #define MAGICK_CONNECT_MAIN_SERVER '\x18'
    #define MAGICK_CONNECT_MAIN_SERVER_RESP '\x14'
    #define MAGICK_INFO_LOBBY '\x15'
    #define MAGICK_CONNECT_LOBBY '\x19'
    #define MAGICK_CONNECT_LOBBY_RESP '\x1a'
    #define MAGICK_DISCONNECT_LOBBY '\x1b'
    #define MAGICK_MISSILE_DEATH '\x1c'
    #define MAGICK_END_GAME '\x1d'
    #define MAGICK_LOBBY_PID '\x1e'

typedef unsigned char n_magick_t;
typedef int n_idsReceived_t;
typedef unsigned char n_nbAction_t;

/**
 * @brief The types of action
 * it is used to know what action is in the packet
 * in order to cast the body of the packet
 */
enum n_actionType_t {
    NO_ACTION = 0,
    NITWORK_INIT = 1,
    NITWORK_READY = 2,
    NITWORK_START_WAVE = 3,
    NITWORK_LIFE_UPDATE = 5,
    NITWORK_ENEMY_DEATH = 6,
    NITWORK_POSITION_RELATIVE = 7,
    NITWORK_POSITION_ABSOLUTE = 8,
    NITWORK_NEW_MISSILE = 9,
    NITWORK_NEW_ENEMY = 10,
    NITWORK_NEW_PLAYER = 11,
    NITWORK_POSITION_RELATIVE_BROADCAST = 12,
    NITWORK_POSITION_ABSOLUTE_BROADCAST = 13,
    NITWORK_PLAYER_DEATH = 14,
    NITWORK_MISSILE_DEATH = 15,
    NITWORK_LIST_LOBBY = 16,
    NITWORK_INFO_LOBBY = 17,
    NITWORK_CONNECT_MAIN_SERVER = 18,
    NITWORK_CONNECT_MAIN_SERVER_RESP = 19,
    NITWORK_CONNECT_LOBBY = 20,
    NITWORK_CONNECT_LOBBY_RESP = 21,
    NITWORK_DISCONNECT_LOBBY = 22,
    NITWORK_CREATE_LOBBY = 23,
    NITWORK_END_GAME = 24,
    NITWORK_LOBBY_PID = 25,
    NITWORK_N_ACTION_TYPE_MAX,
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
PACK(struct msgStartWave_s {
        n_magick_t magick;
        n_id_t waveId;
});

PACK(struct packetMsgStartWave_s {
        struct header_s header;
        struct action_s action;
        struct msgStartWave_s msgStartWave;
});

/* Message Life Update */
PACK(struct msgLifeUpdate_s {
        n_magick_t magick;
        n_id_t playerId;
        struct health_s life;
});

PACK(struct packetLifeUpdate_s {
        struct header_s header;
        struct action_s action;
        struct msgLifeUpdate_s msgLifeUpdate;
});

/* Message Enemy Death */
PACK(struct msgEnemyDeath_s {
        n_magick_t magick;
        struct enemy_id_s enemyId;
});

PACK(struct packetEnemyDeath_s {
        struct header_s header;
        struct action_s action;
        struct msgEnemyDeath_s msgEnemyDeath;
});

/* Message Position Relative */
PACK(struct msgPositionRelative_s {
        n_magick_t magick;
        struct position_relative_s pos;
});

PACK(struct packetPositionRelative_s {
        struct header_s header;
        struct action_s action;
        struct msgPositionRelative_s msg;
});

/* Message New Enemy */
PACK(struct msgNewEnemy_s {
        n_magick_t magick;
        struct enemy_infos_s enemyInfos;
});

PACK(struct packetNewEnemy_s {
    struct header_s header;
    struct action_s action;
    struct msgNewEnemy_s msg;
});

/* Message New Bullet */
PACK(struct msgNewBullet_s {
        n_magick_t magick;
        struct position_absolute_s pos;
        n_id_t id;
        int life;
        missileTypes_e missileType;
});

PACK(struct packetNewBullet_s {
    struct header_s header;
    struct action_s action;
    struct msgNewBullet_s msg;
});

/* Message Position Absolute */
PACK(struct msgPositionAbsolute_s {
        n_magick_t magick;
        struct position_absolute_s pos;
});

PACK(struct packetPositionAbsolute_s {
        struct header_s header;
        struct action_s action;
        struct msgPositionAbsolute_s msg;
});

/* Message New Allie */
PACK(struct msgCreatePlayer_s {
        n_magick_t magick;
        n_id_t playerId;
        struct position_absolute_s pos;
        struct health_s life;
        char isOtherPlayer;
});

PACK(struct packetCreatePlayer_s {
    struct header_s header;
    struct action_s action;
    struct msgCreatePlayer_s msg;
});

/* Message broadcast position relative */
PACK(struct msgPositionRelativeBroadcast_s {
    n_magick_t magick;
    struct position_relative_s pos;
    n_id_t playerId;
});

PACK(struct packetPositionRelativeBroadcast_s {
        struct header_s header;
        struct action_s action;
        struct msgPositionRelativeBroadcast_s msg;
});

/* Message broadcast position absolute */
PACK(struct msgPositionAbsoluteBroadcast_s {
    n_magick_t magick;
    struct position_absolute_s pos;
    n_id_t playerId;
});

PACK(struct packetPositionAbsoluteBroadcast_s {
        struct header_s header;
        struct action_s action;
        struct msgPositionAbsoluteBroadcast_s msg;
});

/* Message Player death */
PACK(struct msgPlayerDeath_s {
        n_magick_t magick;
        n_id_t playerId;
});

PACK(struct packetPlayerDeath_s {
        struct header_s header;
        struct action_s action;
        struct msgPlayerDeath_s msg;
});

/* Message Missile Death */
PACK(struct msgMissileDeath_s {
     n_magick_t magick;
     n_id_t missileId;
});

PACK(struct packetMissileDeath_s {
    struct header_s header;
    struct action_s action;
    struct msgMissileDeath_s msgMissileDeath;
});

 /* Message Request List Lobby */
PACK(struct msgRequestListLobby_s {
    n_magick_t magick;
});

PACK(struct packetRequestListLobby_s {
    struct header_s header;
    struct action_s action;
    struct msgRequestListLobby_s msg;
});

/* Message List Lobby */
PACK(struct connectionData_s {
    char ip[16];
    n_port_t port;
});

PACK(struct lobby_s {
    char name[32];
    unsigned int maxNbPlayer;
    enum gameType_e gameType;
    struct connectionData_s lobbyInfos;
    struct connectionData_s ownerInfos;
});

PACK(struct msgLobbyInfo_s {
    n_magick_t magick;
    struct lobby_s lobby;
});

PACK(struct actionLobby_s {
    struct action_s action;
    struct msgLobbyInfo_s lobby;
});

PACK(struct packetListLobby_s {
    struct header_s header;
    struct actionLobby_s actionLobby[5];
});

/* Message Create Lobby */
PACK(struct msgCreateLobby_s {
    n_magick_t magick;
    char name[32];
    enum gameType_e gameType;
    unsigned int maxNbPlayer;
    struct connectionData_s ownerInfos;
});

PACK(struct packetCreateLobby_s {
    struct header_s header;
    struct action_s action;
    struct msgCreateLobby_s msg;
});

/* Message Hello Lobby */

/**
 * @brief This message is sent by the lobby to the main server to say that it is ready to receive players
 * and to be added to the list of lobbies
 * @param magick The magick of the message in order to check if it is the good message
 * @param lobby The lobby infos
 */
PACK(struct msgInfoLobby_s {
    n_magick_t magick;
    char name[32];
    unsigned int maxNbPlayer;
    enum gameType_e gameType;
    struct connectionData_s ownerInfos;
});

PACK(struct packetInfoLobby_s {
    struct header_s header;
    struct action_s action;
    struct msgInfoLobby_s msg;
});

// ---------------------------------------------------------------------------
// Client Connect to Main Server

PACK(struct msgConnectMainServer_s {
    n_magick_t magick;
});

PACK(struct packetConnectMainServer_s {
    struct header_s header;
    struct action_s action;
    struct msgConnectMainServer_s msg;
});

// Server Respond

PACK(struct msgConnectMainServerResp_s {
    n_magick_t magick;
});

PACK(struct packetConnectMainServerResp_s {
    struct header_s header;
    struct action_s action;
    struct msgConnectMainServerResp_s msg;
});

/* Message Connect Lobby */
// Client Connect to Lobby

PACK(struct msgConnectLobby_s {
    n_magick_t magick;
});

PACK(struct packetConnectLobby_s {
    struct header_s header;
    struct action_s action;
    struct msgConnectLobby_s msg;
});

// Lobby Respond

PACK(struct msgConnectLobbyResp_s {
    n_magick_t magick;
    char isOk;
});

PACK(struct packetConnectLobbyResp_s {
    struct header_s header;
    struct action_s action;
    struct msgConnectLobbyResp_s msg;
});

/* Message Disconnect Lobby */
PACK(struct msgDisconnectLobby_s {
    n_magick_t magick;
});

PACK(struct packetDisconnectLobby_s {
    struct header_s header;
    struct action_s action;
    struct msgDisconnectLobby_s msg;
});

/* Message End Game */
PACK(struct msgEndGame_s {
    n_magick_t magick;
});

PACK(struct packetEndGame_s {
    struct header_s header;
    struct action_s action;
    struct msgEndGame_s msg;
});

/* Message Lobby PID */
PACK(struct msgReplaceLobbyPid_s {
    n_magick_t magick;
    char name[32];
    int pid;
});

PACK(struct packetReplaceLobbyPid_s {
    struct header_s header;
    struct action_s action;
    struct msgReplaceLobbyPid_s msg;
});

#endif
