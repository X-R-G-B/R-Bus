#ifndef MESSAGETYPES_H
    #define MESSAGETYPES_H

    #ifdef __GNUC__
        #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
    #endif

    #ifdef _MSC_VER
        #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
    #endif

typedef unsigned int n_id_t;
typedef int n_port_t;

enum enemy_type_e {
    CLASSIC_ENEMY = 0,
    TERMINATOR
};

enum missileTypes_e {
    CLASSIC = 0,
    FAST,
    BOUNCE,
    PERFORANT
};

enum physicsType_e {
    ZIGZAG = 0,
    BOUNCING
};

PACK(struct health_s {
    int hp;
});

PACK(struct enemy_id_s {
    n_id_t id;
});

PACK(struct position_relative_s {
    char x;
    char y;
});

PACK(struct position_absolute_s {
    int x;
    int y;
});

PACK(struct enemy_infos_s {
    struct enemy_id_s id;
    struct health_s life;
    struct position_absolute_s pos;
    enemy_type_e type;
});

PACK(struct player_infos_s {
    n_id_t id;
    struct position_absolute_s pos;
});

#endif
