#ifndef MESSAGETYPES_H
    #define MESSAGETYPES_H

    #ifdef __GNUC__
        #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
    #endif

    #ifdef _MSC_VER
        #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop))
    #endif

typedef unsigned int n_id_t;

enum enemy_type_e {
    CLASSIC_ENEMY = 0,
};

enum missileTypes_e { CLASSIC };

PACK(struct health_s {
    int hp;
});

PACK(struct enemy_id_s {
    unsigned int id;
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
    unsigned int id;
    struct position_absolute_s pos;
});

#endif
