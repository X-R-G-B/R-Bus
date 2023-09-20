# RFC
## Échange Client->Server
Version: **V0.1.0**


# Sommaire

- Explication de chaque paquet:
  - Structure parent

- Events Envoyés par le Client:
  - Initialisation
  - Ready
  - Tir
  - Déplacement relatif au joueur
  - Position (tous les `n` ticks)
  - Collision entre tir et ennemi
  - Collision entre tir ennemi et sois-même
  - Mort d’un ennemi
  - Mort de sois-même
  - Fin de la partie
  - Fin de la vague


## Explication de chaque paquet


### 1 - Structure parent

```c

typedef int ids_received_t;

typedef int id_t;

typedef char action_type_t;

struct header_s {
    ids_received_t ids_received,
    id_t id,
    nb_action_t nb_action,
} __attribute__((packed));

struct action_header_s {
    action_type_t type;
} __attribute__((packed));
```


## Events Envoyés par le Client

### 1 - Initialisation

```c

typedef char magick_t;

struct body_init_s {
     struct action_header_s header,
     magick_t magick,
} __attribute__((packed));

typedef char any_action;

struct msg_init_s {
    struct header_s header,
    struct body_init_s body; // mais en fait c’est any_action[]
} __attribute__((packed));
```

### 2 - Ready

```c

struct body_ready_s {
    struct action_header_s header,
} __attribute__((packed));

typedef char any_action;

struct msg_ready_s {
    struct header_s header,
    struct body_ready_s body; // mais en fait c’est any_action[]
} __attribute__((packed));
```
