```txt
                    Communication Between Client and Server

Abstract

    This document describes the communication between the client and the
    server for the R-Bus game.
    If you want to implement a custom client or server, please follow
    miticulously these RFC.

Copyright Notice

    This document is licensed under the MIT License with all resources used by
    the R-Bus game.

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
            2.2.2. Action Body
    3. Client Actions
        3.1. Init
        3.2. Ready
    4. Server Actions
        4.1. Client Ready Response [3.2.]
        4.2. 
    5. References

1.  Introduction

    The purpose of this document is to facilitate the implementation of custom
    clients and servers for the R-Bus game, as well as for the developers of
    the R-Bus game to know how to develope further new actions for the game.
    This is intended to be a reference for the developers of the R-Bus game.
    This is not intended to be a reference for the players of the R-Bus game.

2.  Network Packet Global Structure

    The packet are send over the network with the UDP protocol.
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
2.2.1.  Action Header
2.2.2.  Action Body

3.  Client Actions

4.  Server Actions

5.  References
```
