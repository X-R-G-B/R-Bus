```txt
                    Communication Between Client and Server

------------------------------------------------------------------------------

Abstract

    This document describes the communication between the client and the
    server for the R-Bus game.
    If you want to implement a custom client or server, please follow
    miticulously these RFC.

------------------------------------------------------------------------------

Copyright Notice

    This document is licensed under the MIT License with all resources used by
    the R-Bus game.

------------------------------------------------------------------------------

Table of Contents

    1. Introduction
    2. Network Packet Global Structure
        2.1. Header
            2.1.1. Magick1
            2.1.2. Ids Received
            2.1.3. Last Id Received
            2.1.4. Id
            2.1.5. Nb Action
            2.1.6. Magick2
        2.2. Body
            2.2.1. Action Header
                2.2.1.1. Magick
            2.2.2. Action Body
                2.2.2.1. INIT
                    2.2.2.1.1. Client
                        2.2.2.1.1.1. Magick
                    2.2.2.1.2. Server
                        2.2.2.1.2.1. Magick
                        2.2.2.1.2.2. Player ID
                        2.2.2.1.2.3. Position
                        2.2.2.1.2.4. Life
                        2.2.2.1.2.5. Is Other Player
                2.2.2.2. READY
                    2.2.2.2.1. Client
                        2.2.2.2.1.1. Magick
                2.2.2.3. START_WAVE
                    2.2.2.3.1. Server
                        2.2.2.3.1.1. Magick
                        2.2.2.3.1.2. Enemy ID start
                2.2.2.4. LIFE_UPDATE
                    2.2.2.4.1. Client
                        2.2.2.4.1.1. Magick
                        2.2.2.4.1.2. Player ID
                        2.2.2.4.1.3. Life
                            2.2.2.4.1.3.1. HP
                    2.2.2.4.1. Server
                        2.2.2.4.1.1. Magick
                        2.2.2.4.1.2. Player ID
                        2.2.2.4.1.3. Life
                            2.2.2.4.1.3.1. HP
                2.2.2.5. ENEMY_DEATH
                    2.2.2.5.1. Client
                        2.2.2.5.1.1. Magick
                        2.2.2.5.1.2. Enemy
                            2.2.2.5.1.2.1. ID
                    2.2.2.5.2. Server
                        2.2.2.5.2.1. Magick
                        2.2.2.5.2.2. Enemy
                            2.2.2.5.2.2.1. ID
                2.2.2.6. POSITION_RELATIVE
                    2.2.2.6.1. Client
                        2.2.2.6.1.1. Magick
                        2.2.2.6.1.2. Position
                            2.2.2.6.1.2.1. X
                            2.2.2.6.1.2.2. Y
                2.2.2.7. POSITION_ABSOLUTE
                    2.2.2.7.1. Client
                        2.2.2.7.1.1. Magick
                        2.2.2.7.1.2. Position
                            2.2.2.7.1.2.1. X
                            2.2.2.7.1.2.2. Y
                2.2.2.8. NEW_BULLET
                    2.2.2.8.1. Client
                        2.2.2.8.1.1. Magick
                        2.2.2.8.1.2. Position
                            2.2.2.8.1.2.1.  X
                            2.2.2.8.1.2.1.  Y
                        2.2.2.8.1.3. Bullet Type
                2.2.2.9. NEW_ENEMY
                    2.2.2.9.1. Server
                        2.2.2.9.1.1. Magick
                        2.2.2.9.1.2. Enemy
                            2.2.2.9.1.2.1. Enemy ID
                                2.2.2.9.1.2.1.1. ID
                            2.2.2.9.1.2.2. Health
                                2.2.2.9.1.2.2.1. HP
                            2.2.2.9.1.2.3. Position
                                2.2.2.9.1.2.3.1. X
                                2.2.2.9.1.2.3.2. Y
                            2.2.2.9.1.2.4. Enemy Type
                2.2.2.10. NEW_PLAYER
                    2.2.2.10.1. Server
                        2.2.2.10.1.1. Magick
                        2.2.2.10.1.2. Player ID
                        2.2.2.10.1.3. Position
                        2.2.2.10.1.4. Life
                        2.2.2.10.1.5. Is Other Player
                2.2.2.11. POSITION_RELATIVE_BROADCAST
                    2.2.2.11.1. Server
                        2.2.2.11.1.1. Magick
                        2.2.2.11.1.2. Position
                            2.2.2.11.1.2.1. X
                            2.2.2.11.1.2.2. Y
                        2.2.2.11.1.3. Player ID
                2.2.2.12. POSITION_ABSOLUTE_BROADCAST
                    2.2.2.12.1. Server
                        2.2.2.12.1.1. Magick
                        2.2.2.12.1.2. Position
                            2.2.2.12.1.2.1. X
                            2.2.2.12.1.2.2. Y
                        2.2.2.12.1.3. Player ID
                2.2.2.13 PLAYER_DEATH
                    2.2.2.13.1. Client
                        2.2.2.13.1.1. Magick
                        2.2.2.13.1.2. Player ID
                    2.2.2.13.2. Server
                        2.2.2.13.2.1. Magick
                        2.2.2.13.2.2. Player ID
    3. References
        3.1. R-Bus
        3.2. RFC
    4. Appendix
        4.1. Header
            4.1.1. Message
            4.1.1. Network

------------------------------------------------------------------------------

1.  Introduction

    The purpose of this document is to facilitate the implementation of custom
    clients and servers for the R-Bus game, as well as for the developers of
    the R-Bus game to know how to develope further new actions for the game.
    This is intended to be a reference for the developers of the R-Bus game.
    This is not intended to be a reference for the players of the R-Bus game.

------------------------------------------------------------------------------

2.  Network Packet Global Structure

    The packets are compressed using the zstd library before being sent over
    the network using the UDP protocol. Upon receipt, the packets are
    decompressed using the zstd library
    To add some better reliability, each packet sent has a header with some
    information about all the packets received.

2.1.  Header

    The header is composed of the following fields:
    - `magick1`
    - `ids_received`
    - `last_id_received`
    - `id`
    - `nb_action`
    - `magick2`

2.1.1.  Magick1

    This field is used to know the packet received is a packet sent by and for
    the R-Bus game.

    This field must be of size 1 byte.
    This field must be equal to the ascii `\x01`

2.1.2.  Ids Received

    This field is one part of the header that helps to achieve the reliability
    we need to have to ensure all the important packets are received.

    This field must be of size 4 bytes.
    This field is an unsigned integer (so starting from 0 to 2^32)

    To construct these field, here are an example of its implementation in
    pseudo-code:
    - The value of `ids_received` is zero'd
    - Start a loop with a counter that starts at `last_id_received` and end at
    `last_id_received - 16`
    - Shift all the bit of `ids_received` to the left
    - If the packet with the id of the value of the counter is received, add 1
    to the `ids_received`
    - End the loop

2.1.3.  Last Id Received

    This field is one part of the header that helps to achieve the reliability
    we need to have to ensure all the important packets are received.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

    This field need to be set to the last id of the packet received.

2.1.4.  Id

    This field is one part of the header that helps to achieve the reliability
    we need to have to ensure all the important packets are received.

    This field must be of size 4 bytes.
    This field is an unsigned integer (so starting from 0 to 2^32)

    This field will start from 0 at start of the connection, and each time a
    packet is sent, it will be incremented by 1.
    When the maximum id is reached, it will be reset to 0.

2.1.5.  Nb Action

    This field is used to know how many action have been sent in the same packet.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)

2.1.6.  Magick2

    This field is used to know the packet received is a packet sent by and for
    the R-Bus game.

    This field must be of size 1 byte.
    This field must be equal to the ascii `\x03`

2.2.  Body

    The body is composed of `n` actions. The number of actions that have
    been sent is in the `nb_action` field in the header of the packet.

2.2.1.  Action Header

    The action header is composed of the following fields:
    - `magick`

2.2.1.1.    Magick

    This field describe how to interpret the action body.

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of these value:
    - NO_ACTION = 0,
    - INIT = 1,
    - READY = 2,
    - START_WAVE = 3,
    - LIFE_UPDATE = 5,
    - ENEMY_DEATH = 6,
    - POSITION_RELATIVE = 7,
    - POSITION_ABSOLUTE = 8,
    - NEW_BULLET = 9,
    - NEW_ENEMY = 10,
    - NEW_PLAYER = 11,
    - POSITION_RELATIVE_BROADCAST = 12,
    - POSITION_ABSOLUTE_BROADCAST = 13,
    - PLAYER_DEATH = 14,

2.2.2.  Action Body

2.2.2.1.    INIT

    The Client must send an init action before everything else.
    The Server respond to this action with an init action.

2.2.2.1.1.  Client

    The Client action contains the following fields:
    - `magick`

2.2.2.1.1.1.    Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x06`

2.2.2.1.2.  Server

    The Server action contains the following fields:
    - `magick`
    - `player_id`
    - `pos`
    - `life`
    - `isOtherPlayer`

2.2.2.1.2.1.    Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x06`

2.2.2.1.2.2.    Player ID

    This field correspond to the ID of the client.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each client

2.2.2.1.2.3.    Position

    This field correspond to the position of the client and contains the
    following fields:
    - `x`
    - `y`

2.2.2.1.2.3.1. X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.1.2.3.2. Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.1.2.4.    Life

    This field correspond to the life of the client and contains the following
    fields:
    - `hp` This field must be of size 4 byte. This field is signed (so starting
    from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.1.2.5.    Is Other Player

    This field correspond to the fact that the client is an other player or not

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to one of these value:
    - TRUE = 1,
    - FALSE = 0, 

2.2.2.2.    READY

    The Client must send a ready action to enter/start the game.
    The Server respond to this action by sending a start wave action, only if
    all the clients are ready

2.2.2.2.1.  Client

    The Client action contains the following fields:
    - `magick`

2.2.2.2.1.1.    Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x17`

2.2.2.3.    START_WAVE

    The Server must send a start wave action before creating enemies.

2.2.2.3.1.  Server

    The Server action contains the following fields:
    - `magick`
    - `enemy_id_start`

2.2.2.3.1.1.    Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x07`

2.2.2.3.1.2.    Enemy ID start

    This field correspond to the ID of the first enemy that will be created.
    After Each enemy created, the ID will be incremented.
    This give the ability to not send the ID of the created enemy at each
    creation.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.4.    LIFE_UPDATE

    The Client must send a life update action each time its life is modified.
    The Server must send a life update action each time a life of player is
    modified.

2.2.2.4.1.  Client

    The Client action contains the following fields:
    - `magick`
    - `player_id`
    - `life`

2.2.2.4.1.1.    Magick

    This field help to know the packet is realy a life update action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0b`

2.2.2.4.1.2.    Player ID

    This field correspond to the ID of the player.

    It will ne be taken into account because the server know the id of the
    client with its connection.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.4.1.3.    Life

    This field correspond to the life of the player and contains the following
    fields:
    - `hp`

2.2.2.4.1.3.1.  HP

    This field correspond to the life of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.4.1.  Server

    The Server action contains the following fields:
    - `magick`
    - `player_id`
    - `life`

2.2.2.4.1.1.    Magick

    This field help to know the packet is realy a life update action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0b`

2.2.2.4.1.2.    Player ID

    This field correspond to the ID of the player that has its life modified.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.4.1.3.    Life

    This field correspond to the life of the player and contains the following
    fields:
    - `hp`

2.2.2.4.1.3.1.  HP

    This field correspond to the life of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.5.    ENEMY_DEATH

    The Client must send a enemy death action each time an enemy is killed.
    The Server must send a enemy death action each time an enemy is killed.

2.2.2.5.1.  Client

    The Client action contains the following fields:
    - `magick`
    - `enemy_id`

2.2.2.5.1.1.    Magick

    This field help to know the packet is realy an enemy death action.

2.2.2.5.1.2.    Enemy

    This field correspond to the ID of the killed enemy and contains the
    following fields:
    - `id`

2.2.2.5.1.2.1.  ID

    This field correspond to the ID of the killed enemy.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.5.2.  Server

    The Server action contains the following fields:
    - `magick`
    - `enemy_id`

2.2.2.5.2.1.    Magick

    This field help to know the packet is realy an enemy death action.

2.2.2.5.2.2.    Enemy

    This field correspond to the ID of the killed enemy and contains the
    following fields:
    - `id`

2.2.2.5.2.2.1.  ID

    This field correspond to the ID of the killed enemy.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.6.    POSITION_RELATIVE

    The Client must send a position relative action every 10 milliseconds.

    Relative means that it is the difference between the the current position
    and the previous position send 10 milliseconds before.

2.2.2.6.1.  Client

    The Client action contains the following fields:
    - `magick`
    - `position`

2.2.2.6.1.1.    Magick

    This field help to know the packet is realy a position relative action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x08`

2.2.2.6.1.2.    Position

    This field correspond to the difference between the current position of
    the client and the position of the client 10 milliseconds before.

    This field contains the following fields:
    - `x`
    - `y`

2.2.2.6.1.2.1.  X

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

2.2.2.6.1.2.2.  Y

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

2.2.2.7.    POSITION_ABSOLUTE

    The Client must send a position absolute action every 1 seconds.

    Absolute means that it is the current position of the client.

2.2.2.7.1.  Client

    The Client action contains the following fields:
    - `magick`
    - `position`

2.2.2.7.1.1.    Magick

    This field help to know the packet is realy a position absolute action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x09`

2.2.2.7.1.2.    Position

    This field correspond to the current position of the client.

    This field contains the following fields:
    - `x`
    - `y`

2.2.2.7.1.2.1.  X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.7.1.2.2.  Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.8.    NEW_BULLET

    The Client must send a new bullet action when he launch a bullet.

2.2.2.8.1.  Client

    The Client action contains the following fields:
    - `magick`
    - `position`
    - `bullet_type`

2.2.2.8.1.1.    Magick

    This field help to know the packet is realy a new bullet action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0c`

2.2.2.8.1.2.    Position

    This field correspond to the absolute creation position of the bullet.

2.2.2.8.1.2.1.  X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.8.1.2.2.  Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.8.1.3.    Bullet Type

    This field correspond to the type of the bullet.

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of these value:
    - CLASSIC = 0,

2.2.2.9.   NEW_ENEMY

    The Server must send a new enemy action when he create an enemy.

2.2.2.9.1.     Server

    The Server action contains the following fields:
    - `magick`
    - `enemy`

2.2.2.9.1.1.   Magick

    This field help to know the packet is realy a new enemy action.

2.2.2.9.1.2.   Enemy

    This field correspond to the new enemy and contains the following fields:
    - `id`
    - `position`
    - `enemy_type`

2.2.2.9.1.2.1.     Enemy ID

    This field correspond to the ID of the new enemy and contains the following
    fields:
    - `id`

2.2.2.9.1.2.1.1.   ID

    This field correspond to the ID of the new enemy.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.9.1.2.2.     Health

    This field correspond to the health of the new enemy and contains the
    following fields:
    - `health`

2.2.2.9.1.2.2.1.   HP

    This field correspond to the health of the new enemy.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.9.1.2.3.     Position

    This field correspond to the absolute position of the new enemy and
    contains the following fields:
    - `x`
    - `y`

2.2.2.9.1.2.3.1.   X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.9.1.2.3.2.   Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.9.1.2.4.     Enemy Type

    This field correspond to the type of the new enemy.

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of these value:
    - CLASSIC_ENEMY = 0

2.2.2.10.   NEW_PLAYER

    The Server must send a new allie action when a new player enter in the
    game.

2.2.2.10.1.     Server

    The Server action contains the following fields:
    - `magick`
    - `playerId`
    - `pos`
    - `life`
    - `isOtherPlayer`

2.2.2.10.1.1.    Magick

    This field help to know the packet is realy a new allie action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0a`

2.2.2.10.1.2.    Player ID

    This field correspond to the ID of the client.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each client

2.2.2.10.1.3.    Position

    This field correspond to the position of the client and contains the
    following fields:
    - `x` This field must be of size 4 byte. This field is signed (so starting
    from -((2^32)/2) to +(((2^32)/2)-1))
    - `y` This field must be of size 4 byte. This field is signed (so starting
    from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.10.1.3.1. X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.10.1.3.2. Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.10.1.4.    Life

    This field correspond to the life of the client and contains the following
    fields:
    - `hp` This field must be of size 4 byte. This field is signed (so starting
    from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.10.1.5.    Is Other Player

    This field correspond to the fact that the client is an other player or not

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to one of these value:
    - TRUE = 1,
    - FALSE = 0, 

2.2.2.11.   POSITION_RELATIVE_BROADCAST

    The Server can send a position relative broadcast to all the players when a
    player send a position relative action.
    It is up to the server to decide if he broadcast or not.

2.2.2.11.1.     Server

    The Server action contains the following fields:
    - `magick`
    - `position`
    - `player_id`

2.2.2.11.1.1.   Magick

    This field help to know the packet is realy a position relative broadcast
    action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0f`

2.2.2.11.1.2.   Position

    This field correspond to the relative position of the player and contains
    the following fields:
    - `x`
    - `y`

2.2.2.11.1.2.1.     X

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

2.2.2.11.1.2.2.     Y

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

2.2.2.11.1.3.   Player ID

    This field correspond to the ID of the player that has its position
    modified.

2.2.2.12.   POSITION_ABSOLUTE_BROADCAST

    The Server can send a position absolute broadcast to all the players when a
    player send a position absolute action.
    It is up to the server to decide if he broadcast or not.

2.2.2.12.1.     Server

    The Server action contains the following fields:
    - `magick`
    - `position`
    - `player_id`

2.2.2.12.1.1.   Magick

    This field help to know the packet is realy a position absolute broadcast
    action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x10`

2.2.2.12.1.2.   Position

    This field correspond to the absolute position of the player and contains
    the following fields:
    - `x`
    - `y`

2.2.2.12.1.2.1.     X

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.12.1.2.2.     Y

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.12.1.3.   Player ID

    This field correspond to the ID of the player that has its position
    modified.

2.2.2.13.   PLAYER_DEATH

    The Client must send a player death action when he die. Or any other player
    The Server must send a player death action when a player die. And check if 
    a player is dead when he receive a PLAYER_DEATH action from a client.

2.2.2.13.1.    Client

    The Client action contains the following fields:
    - `magick`
    - `player_id`

2.2.2.13.1.1.  Magick

    This field help to know the packet is realy a player death action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x11`

2.2.2.13.1.2.  Player ID

    This field correspond to the ID of the player that has died.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

2.2.2.13.2.    Server

    The Server action contains the following fields:
    - `magick`
    - `player_id`

2.2.2.13.2.1.  Magick

    This field help to know the packet is realy a player death action.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x11`

2.2.2.13.2.2.  Player ID

    This field correspond to the ID of the player that has died.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

3.  References

    Link to some mentioned word

3.1. R-Bus

    - The R-Bus game project source code <https://github.com/X-R-G-B/R-Bus>
    - The R-Bus game documentation <https://x-r-g-b.github.io/R-Bus/>

3.2. RFC

    - How to write an RFC <https://www.rfc-editor.org/rfc/rfc7322>

4. Appendix

    Files that helps understand the RFC

4.1. Header

    Header files used by the R-Bus game

4.1.1. Message

    Structures used by the R-Bus game inside the ECS
    <https://github.com/X-R-G-B/R-Bus/blob/dev/src/ECS/MessageTypes.h>

4.1.1. Network

    Structures used by the R-Bus game inside the network
    <https://github.com/X-R-G-B/R-Bus/blob/dev/src/Nitwork/Nitwork.h>

```
