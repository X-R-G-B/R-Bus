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
        2.2. Body
            2.2.1. Action Header
            2.2.2. Action Body
    3. Network Packet Actions
        3.1. Client -> Main Server
            3.1.1. CONNECT_MAIN_SERVER
            3.1.2. LIST_LOBBY
            3.1.3. CREATE_LOBBY
        3.2. Client -> Lobby Server
            3.2.1. CONNECT_LOBBY
            3.2.2. INIT
            3.2.3. READY
            3.2.4. POSITION_RELATIVE
            3.2.5. POSITION_ABSOLUTE
            3.2.6. NEW_MISSILE
            3.2.7. LIFE_UPDATE
            3.2.8. ENEMY_DEATH
            3.2.9. PLAYER_DEATH
            3.2.10. MISSILE_DEATH
            3.2.11. DISCONNECT_LOBBY

        3.3. Main Server -> Client
            3.3.1. CONNECT_MAIN_SERVER_RESP
            3.3.2. LIST_LOBBY
        3.4. Lobby Server -> Client
            3.4.1. CONNECT_LOBBY_RESP
            3.4.2. START_WAVE
            3.4.3. LIFE_UPDATE
            3.4.4. ENEMY_DEATH
            3.4.5. NEW_ENEMY
            3.4.6. NEW_PLAYER
            3.4.6. NEW_MISSILE
            3.4.8. POSITION_ABSOLUTE_BROADCAST
            3.4.9. POSITION_RELATIVE_BROADCAST
            3.4.10. PLAYER_DEATH
            3.4.11. MISSILE_DEATH
        3.5. Lobby Server -> Main Server
            3.5.1. INFO_LOBBY
    4. References
        3.1. R-Bus
        3.2. RFC
    5. Appendix
        5.1. Header
            5.1.1. Message
            5.1.1. Network

------------------------------------------------------------------------------

1. Introduction

    The purpose of this document is to facilitate the implementation of custom
    clients and servers for the R-Bus game, as well as for the developers of
    the R-Bus game to know how to develope further new actions for the game.
    This is intended to be a reference for the developers of the R-Bus game.
    This is not intended to be a reference for the players of the R-Bus game.

------------------------------------------------------------------------------

2. Network Packet Global Structure

    The packets are compressed using the zstd library before being sent over
    the network using the UDP protocol. Upon receipt, the packets are
    decompressed using the zstd library
    To add some better reliability, each packet sent has a header with some
    information about all the packets received.

2.1. Header

    The header is composed of the following fields:
    - `magick1`
    - `ids_received`
    - `last_id_received`
    - `id`
    - `nb_action`
    - `magick2`

    *** Magick1

    This field is used to know the packet received is a packet sent by and for
    the R-Bus game.

    This field must be of size 1 byte.
    This field must be equal to the ascii `\x01`

    *** Ids Received

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

    *** Last Id Received

    This field is one part of the header that helps to achieve the reliability
    we need to have to ensure all the important packets are received.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

    This field need to be set to the last id of the packet received.

    *** Id

    This field is one part of the header that helps to achieve the reliability
    we need to have to ensure all the important packets are received.

    This field must be of size 4 bytes.
    This field is an unsigned integer (so starting from 0 to 2^32)

    This field will start from 0 at start of the connection, and each time a
    packet is sent, it will be incremented by 1.
    When the maximum id is reached, it will be reset to 0.

    *** Nb Action

    This field is used to know how many action have been sent in the same packet.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)

    *** Magick2

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

    *** Magick

    This field describe how to interpret the action body.

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

2.2.2.  Action Body

    The next bytes in the packet are specified in the list of available
    actions.

------------------------------------------------------------------------------

3. Network Packet Actions

    This section specify each action that a Client, Main Server or Lobby Server
    can send.

3.1. Client -> Main Server

3.1.1. CONNECT_MAIN_SERVER

    To understand this action, the action header `magick` must be equal to `19`

    The action body is composed of the following fields:
    - `magick`

    *** Magick

    This field help to know the packet is realy a connect action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x18`

3.1.2. LIST_LOBBY

    To understand this action, the action header `magick` must be equal to `16`

    The action body is composed of the following fields:
    - `magick`

    *** Magick

    This field help to know the packet is realy a list lobby action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x16`

3.1.3. CREATE_LOBBY

    To understand this action, the action header `magick` must be equal to `23`

    The action body is composed of the following fields:
    - `magick`
    - `name`
    - `gameType`
    - `maxNbPlayer`
    - `ip`
    - `port`

    *** Magick

    This field help to know the packet is realy a create lobby action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x13`

    *** Name

    This field correspond to the name of the lobby.

    This field must be of size 32 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 32 bytes.

    *** Game Type

    This field correspond to the type of the game created.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic game

    *** Max Nb Player

    This field correspond to the maximum number of player in the lobby.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

    *** IP

    This field correspond to the ip of the Main Server.

    This field must be of size 16 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 16 bytes.

    *** Port

    This field correspond to the port of the Main Server.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

3.2. Client -> Lobby Server

3.2.1. CONNECT_LOBBY

    To understand this action, the action header `magick` must be equal to `20`

    The action body is composed of the following fields:
    - `magick`
    
    *** Magick
    
    This field help to know the packet is realy a connect lobby action
    
    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x19`

3.2.2. INIT

    To understand this action, the action header `magick` must be equal to `1`

    This packet initialize the connection to the lobby, and by extension,
    register to the game that will be created in that lobby.

    The Lobby Server will respond to this packet with a `NEW_PLAYER` (3.4.5.)
    action.

    The action body is composed of the following fields:
    - `magick`

    *** Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x06`

3.2.3. READY

    To understand this action, the action header `magick` must be equal to `2`

    This packet tell the Lobby Server that the client is ready to play.
    The Lobby Server will wait that all players connected are ready to start
    the game.

    The action body is composed of the following fields:
    - `magick`

    *** Magick

    This field help to know the packet is realy a ready action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x17`

3.2.4. POSITION_RELATIVE

    To understand this action, the action header `magick` must be equal to `7`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`

    *** Magick

    This field help to know the packet is realy a position relative action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x08`

    *** X

    This field correspond to the difference between the current position of
    the client and the position of the client 10 milliseconds before.

    This field must be of size 1 bytes.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

    *** Y

    This field correspond to the difference between the current position of
    the client and the position of the client 10 milliseconds before.

    This field must be of size 1 bytes.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

3.2.5. POSITION_ABSOLUTE

    To understand this action, the action header `magick` must be equal to `8`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`

    *** Magick

    This field help to know the packet is realy a position absolute action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x09`

    *** X

    This field correspond to the absolute position of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute position of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

3.2.6. NEW_MISSILE

    To understand this action, the action header `magick` must be equal to `9`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`
    - `missileId`
    - `missileHealth`
    - `missileType`

    *** Magick

    This field help to know the packet is realy a new bullet action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0d`

    *** X

    This field correspond to the absolute position of the bullet when created.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute position of the bullet when created.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Missile ID
    
    This field correspond to the ID of the missile.
    This is useless because this is the server that will set the ID of the
    missile.
    
    This field must be of size 4 byte.
    This field must be equal to `0`
    
    *** Missile Health
    
    This field correspond to the health of the missile.
    This is useless because this is the server that will set the health of the
    missile.
    
    This field must be of size 4 byte.
    This field must be equal to `0`

    *** Missile Type

    This field correspond to the type of the missile.

    This field must be of size 4 byte.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic
    - 1 : fast
    - 2 : bounce
    - 3 : perforant

3.2.7. LIFE_UPDATE

    To understand this action, the action header `magick` must be equal to `5`

    The action body is composed of the following fields:
    - `magick`
    - `playerId`
    - `hp`

    *** Magick

    This field help to know the packet is realy a life update action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0b`

    *** Player ID

    This field correspond to the ID of the client (the server will verify it).

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each client.

    *** HP

    This field correspond to the life of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

3.2.8. ENEMY_DEATH

    To understand this action, the action header `magick` must be equal to `6`

    The action body is composed of the following fields:
    - `magick`
    - `ennemyId`

    *** Magick

    This field help to know the packet is realy an enemy death action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0c`

    *** Ennemy ID

    This field correspond to the ID of the ennemy that has been killed.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each ennemy.

3.2.9. PLAYER_DEATH

    To understand this action, the action header `magick` must be equal to `14`

    The action body is composed of the following fields:
    - `magick`
    - `playerId`

    *** Magick

    This field help to know the packet is realy a player death action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x11`

    *** Player ID

    This field correspond to the ID of the player that has been killed.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each player.

3.2.10. MISSILE_DEATH

    To understant This action, the aciton header `magick` must be equal to `15`
    
    The action body is composed of ther following fields:
    - `magick`
    - `missileId`

    *** Magick
   
    This field help to know the packet is realy missile death action
    
    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x1c`
    
    *** Missile ID
    
    This field correspond to the ID of the missile that died

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each missile.

3.2.11. DISCONNECT_LOBBY

    To understant this aciton, the action header `magick` must be equal to `22`
    
    The action body is composed of the following fields:
    - `magick`
    
    *** Magick
    
    This field help to know the packet is realy a disconnect lobby action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x1b`

3.3. Main Server -> Client

3.3.1. CONNECT_MAIN_SERVER_RESP

    To understand this action, the action header `magick` must be equal to `20`

    The action body is composed of the following fields:
    - `magick`

    *** Magick

    This field help to know the packet is realy a connect main server response

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x14`

3.3.2. LIST_LOBBY

    To understand this action, the action header `magick` must be equal to `20`

    The action body is composed of the following fields:
    - `magick`
    - `name`
    - `maxNbPlayer`
    - `gameType`
    - `lobbyIp`
    - `lobbyPort`
    - `ownerIp`
    - `ownerPort`

    *** Magick

    This field help to know the packet is realy a list lobby action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x12`

    *** Name

    This field correspond to the name of the lobby.

    This field must be of size 32 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 32 bytes.

    *** Max Nb Player

    This field correspond to the maximum number of player in the lobby.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

    *** Game Type

    This field correspond to the type of the game created.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic game

    *** Lobby IP

    This field correspond to the ip of the Lobby Server.

    This field must be of size 16 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 16 bytes.

    *** Lobby Port

    This field correspond to the port of the Lobby Server.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Owner IP

    This field correspond to the ip of the Main Server.

    This field must be of size 16 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 16 bytes.

    *** Owner Port

    This field correspond to the port of the Main Server.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

3.4. Lobby Server -> Client

3.4.1. CONNECT_LOBBY_RESP

    To understand this action, the action header `magick` must be equal to `21`

    The action body is composed of the following fields:
    - `magick`
    - `isOk`

    *** Magick

    This field help to know the packet is realy a connect lobby response

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x1a`
    
    *** Is Ok
    
    This field correspond to the status of the connection to the lobby
    
    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field is equal to `1` if the connection is ok
    This field is equal to `0` if the connection is not ok

3.4.2. START_WAVE

    To understand this action, the action header `magick` must be equal to `3`

    The action body is composed of the following fields:
    - `magick`
    - `ennemyId`

    *** Magick

    This field help to know the packet is realy a start wave action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x07`

    *** Ennemy Id

    This field correspond to the id of the first ennemy that will spawn and
    that is part of the wave.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each ennemy.

3.4.3. LIFE_UPDATE

    To understand this action, the action header `magick` must be equal to `5`

    The action body is composed of the following fields:
    - `magick`
    - `playerId`
    - `hp`

    *** Magick

    This field help to know the packet is realy a life update action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0b`

    *** Player ID

    This field correspond to the ID of the client whose life has been updated.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each player.

    *** HP

    This field correspond to the life of the player.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

3.4.4. ENEMY_DEATH

    To understand this action, the action header `magick` must be equal to `6`

    The action body is composed of the following fields:
    - `magick`
    - `enemyId`

    *** Magick

    This field help to know the packet is realy an enemy death action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0c`

    *** Enemy ID

    This field correspond to the id of the ennemy that has been killed.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each ennemy.

3.4.5. NEW_ENEMY

    To understand this action, the action header `magick` must be equal to `10`

    The action body is composed of the following fields:
    - `magick`
    - `ennemyId`
    - `hp`
    - `x`
    - `y`
    - `ennemyType`

    *** Magick

    This field help to know the packet is realy a new enemy action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0e`

    *** Ennemy ID

    This field correspond to the id of the new ennemy.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each ennemy.

    *** HP

    This field correspond to the life of the new ennemy.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** X

    This field correspond to the absolute x position of the new ennemy.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute y position of the new ennemy.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Ennemy Type

    This field correspond to the type of the new ennemy.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic
    - 1 : terminator

3.4.6. NEW_PLAYER

    To understand this action, the action header `magick` must be equal to `11`

    The action body is composed of the following fields:
    - `magick`
    - `playerId`
    - `x`
    - `y`
    - `hp`
    - `isOtherPlayer`

    *** Magick

    This field help to know the packet is realy an init action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0a`

    *** Player ID

    This field correspond to the ID of the client that joined.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each client.

    *** X

    This field correspond to the absolute x position of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute y position of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** HP

    This field correspond to the life of the client.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** isOtherPlayer

    This field is an information to let you know if this player is you.

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field is equal to `1` if it is you
    This field is equal to `0` if it is not you

3.4.7. NEW_MISSILE

    To understand this action, the action header `magick` must be equal to `9`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`
    - `missileId`
    - `missileHealth`
    - `missileType`

    *** Magick

    This field help to know the packet is realy a new bullet action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0d`

    *** X

    This field correspond to the absolute x position of the bullet.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute y position of the bullet.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Missile ID
    
    This field correspond to the ID of the missile.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    
    *** Missile Health
    
    This field correspond to the health of the missile.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Missile Type

    This field correspond to the type of the bullet.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic
    - 1 : fast
    - 2 : bounce
    - 3 : perforant

3.4.8. POSITION_ABSOLUTE_BROADCAST

    To understand this action, the action header `magick` must be equal to `13`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`
    - `playerId`

    *** Magick

    This field help to know the packet is realy a position absolute broadcast
    action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x10`

    *** X

    This field correspond to the absolute x position of the bullet.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Y

    This field correspond to the absolute y position of the bullet.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

    *** Player Id

    This field correspond to the id of the player.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each player.

3.4.9. POSITION_RELATIVE_BROADCAST

    To understand this action, the action header `magick` must be equal to `12`

    The action body is composed of the following fields:
    - `magick`
    - `x`
    - `y`
    - `playerId`

    *** Magick

    This field help to know the packet is realy a position relative broadcast
    action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x0f`

    *** X

    This field correspond to the relative x position of the bullet since last
    packet send.

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

    *** Y

    This field correspond to the relative y position of the bullet since last
    packet send.

    This field must be of size 1 byte.
    This field is signed (so starting from -((2^8)/2) to +(((2^8)/2)-1))

    *** Player Id

    This field correspond to the id of the player.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each player.

3.4.10. PLAYER_DEATH

    To understand this action, the action header `magick` must be equal to `14`

    The action body is composed of the following fields:
    - `magick`
    - `playerId`

    *** Magick

    This field help to know the packet is realy a player death action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x11`

    *** Player Id

    This field correspond to the id of the player.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each player.

3.4.11. MISSILE_DEATH

    To understant This action, the aciton header `magick` must be equal to `15`
    
    The action body is composed of ther following fields:
    - `magick`
    - `missileId`

    *** Magick
   
    This field help to know the packet is realy missile death action
    
    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x16`
    
    *** Missile ID
    
    This field correspond to the ID of the missile that died

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)
    This field is unique for each missile.

3.5. Lobby Server -> Main Server

3.5.1. INFO_LOBBY

    To understand this action, the action header `magick` must be equal to `18`

    This action is used to let the Main Server know the current Lobby Server
    exists.

    The action body is composed of the following fields:
    - `magick`
    - `name`
    - `maxNbPlayer`
    - `gameType`
    - `ip`
    - `port`

    *** Magick

    This field help to know the packet is realy an info lobby action

    This field must be of size 1 byte.
    This field is unsigned (so starting from 0 to 2^8)
    This field must be equal to the ascii `\x15`

    *** Name

    This field correspond to the name of the lobby.

    This field must be of size 32 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 32 bytes.

    *** Max Nb Player

    This field correspond to the maximum number of player in the lobby.

    This field must be of size 4 bytes.
    This field is unsigned (so starting from 0 to 2^32)

    *** Game Type

    This field correspond to the type of the game created.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))
    This field must be equal to one of this value:
    - 0 : classic game

    *** IP

    This field correspond to the ip of the Main Server.

    This field must be of size 16 bytes.
    Each byte is a signed integer (so starting from -((2^8)/2) to
    +(((2^8)/2)-1)).
    Each byte correspond to the ascii character.
    After the last byte that you set, you must add a null character.
    The null character is the ascii character `\x00`
    The null character must be on the range of the 16 bytes.

    *** Port

    This field correspond to the port of the Main Server.

    This field must be of size 4 bytes.
    This field is signed (so starting from -((2^32)/2) to +(((2^32)/2)-1))

------------------------------------------------------------------------------

4.  References

    Link to some mentioned word

4.1. R-Bus

    - The R-Bus game project source code <https://github.com/X-R-G-B/R-Bus>
    - The R-Bus game documentation <https://x-r-g-b.github.io/R-Bus/>

4.2. RFC

    - How to write an RFC <https://www.rfc-editor.org/rfc/rfc7322>

------------------------------------------------------------------------------

5. Appendix

    Files that helps understand the RFC

5.1. Header

    Header files used by the R-Bus game

5.1.1. Message

    Structures used by the R-Bus game inside the ECS
    <https://github.com/X-R-G-B/R-Bus/blob/dev/src/ECS/MessageTypes.h>

5.1.1. Network

    Structures used by the R-Bus game inside the network
    <https://github.com/X-R-G-B/R-Bus/blob/dev/src/Nitwork/Nitwork.h>

```