/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Internal header for NitworkServer C
*/


#ifndef NITWORK_H
    #define NITWORK_H

    #include "MessageTypes.h"

    #define HEADER_SIZE sizeof(struct header_s)
    #define TICKS 128
    #define TICKS_PER_SECOND(t) (t / TICKS)
    #define TICKS_PER_MILLISECOND(t) (TICKS_PER_SECOND(t) / 1000)
    #define DEFAULT_THREAD_NB 4
    #define MAX_NB_ACTION 16
    #define HEADER_CODE1 '\x01'
    #define HEADER_CODE2 '\x03'

    #define MAGICK_INIT '\x06'
    #define MAGICK_READY '\x17'
    #define MAGICK_START_WAVE '\x07'
    #define MAGICK_POSITION_RELATIVE '\x08'
    #define MAGICK_POSITION_ABSOLUTE '\x09'
    #define MAGICK_LIFE_UPDATE '\x0b'
    #define MAGICK_ENEMY_DEATH '\x0c'
    #define MAGICK_NEW_ENEMY '\x0e'
    #define MAGICK_NEW_BULLET '\x0d'
    #define MAGICK_POSITION_RELATIVE_BROADCAST '\x0f'
    #define MAGICK_POSITION_ABSOLUTE_BROADCAST '\x10'
    #define MAGICK_NEW_PLAYER '\x0a'
    #define MAGICK_PLAYER_DEATH '\x11'

typedef unsigned char n_magick_t;
typedef int n_idsReceived_t;
typedef unsigned char n_nbAction_t;

enum n_actionType_t {
    NO_ACTION = 0,
    INIT = 1,
    READY = 2,
    START_WAVE = 3,
    LIFE_UPDATE = 5,
    ENEMY_DEATH = 6,
    POSITION_RELATIVE = 7,
    POSITION_ABSOLUTE = 8,
    NEW_BULLET = 9,
    NEW_ENEMY = 10,
    NEW_PLAYER = 11,
    POSITION_RELATIVE_BROADCAST = 12,
    POSITION_ABSOLUTE_BROADCAST = 13,
    PLAYER_DEATH = 14,
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
PACK(struct msgStartWave_s {
        n_magick_t magick;
        n_id_t enemyNb;
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
        missileTypes_e missileType;
        unsigned long int timestamp;
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
        bool isOtherPlayer;
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

#endif
