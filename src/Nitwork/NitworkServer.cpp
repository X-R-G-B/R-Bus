/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkServer
*/

#include "NitworkServer.hpp"
#include "ECSCustomTypes.hpp"
#include "Logger.hpp"
#include "Registry.hpp"
#include "SystemManagersDirector.hpp"
#include "Systems.hpp"

namespace Nitwork {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    NitworkServer NitworkServer::_instance = NitworkServer();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    NitworkServer &NitworkServer::getInstance()
    {
        return _instance;
    }

    bool NitworkServer::startServer(
        int nbPlayer,
        enum gameType_e gameType,
        const std::string &name,
        const std::string &ownerIp,
        int ownerPort,
        int port,
        int threadNb,
        int tick)
    {
        auto startStatus = ANitwork::start(port, threadNb, tick, "");

        _serverInfos.maxNbPlayer = nbPlayer;
        std::strcpy(_serverInfos.name, (name.size() > 32) ? name.substr(0, 32).data() : name.data());
        std::strcpy(
            _serverInfos.ownerInfos.ip,
            (ownerIp.size() > 16) ? ownerIp.substr(0, 16).data() : ownerIp.data());
        _serverInfos.ownerInfos.port = ownerPort;
        _serverInfos.lobbyInfos.port = _socket.local_endpoint().port();
        std::strcpy(
            _serverInfos.lobbyInfos.ip,
            (_socket.local_endpoint().address().to_string().size() > 16)
                ? _socket.local_endpoint().address().to_string().substr(0, 16).data()
                : _socket.local_endpoint().address().to_string().data());
        _serverInfos.gameType = gameType;
        // TODO: add gameType to lobby
        return startStatus;
    }

    struct lobby_s NitworkServer::getServerInfos() const
    {
        return _serverInfos;
    }

    bool NitworkServer::startNitworkConfig(int port, const std::string & /* unused */)
    {
        boost::asio::ip::udp::endpoint endpoint =
            boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), boost::asio::ip::port_type(port));

        _socket.open(boost::asio::ip::udp::v4());
        if (!_socket.is_open()) {
            Logger::error("Socket not open");
            return false;
        }
        _socket.bind(endpoint);
        return true;
    }

    void NitworkServer::handleBodyAction(
        const struct header_s &header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data());
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)

        auto it = _actionsHandlers.find(action->magick);
        std::memmove(
            _receiveBuffer.data(),
            _receiveBuffer.data() + sizeof(struct action_s),
            _receiveBuffer.size() - sizeof(struct action_s));
        std::memset(
            _receiveBuffer.data() + _receiveBuffer.size() - sizeof(struct action_s),
            0,
            sizeof(struct action_s));

        if (it == _actionsHandlers.end()) {
            Logger::error("Error: action not found");
            return;
        }
        if (it->second.first == nullptr) {
            Logger::error("Error: action handler is null");
            return;
        }
        Logger::debug(
            "Received packet from " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port())
            + " with id " + std::to_string(header.id) + " and action of type "
            + std::to_string(action->magick));
        it->second.first(it->second.second, header);
    }

    /* Getters Section */
    const std::map<enum n_actionType_t, actionSender> &NitworkServer::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }
    /* End Getters Section */

    void NitworkServer::sendNewAllie(
        n_id_t playerId,
        struct packetCreatePlayer_s packetMsgCreatePlayer,
        boost::asio::ip::udp::endpoint &endpoint,
        bool butNoOne)
    {
        packetMsgCreatePlayer.msg.playerId = playerId;
        if (butNoOne) {
            Packet packet(
                packetMsgCreatePlayer.action.magick,
                std::make_any<struct packetCreatePlayer_s>(packetMsgCreatePlayer));
            sendToAllClientsButNotOne(packet, endpoint);
        } else {
            Packet packet(
                packetMsgCreatePlayer.action.magick,
                std::make_any<struct packetCreatePlayer_s>(packetMsgCreatePlayer),
                endpoint);
            addPacketToSend(packet);
        }
    }

    /* Handle packet (msg) Section */
    void NitworkServer::sendAlliesAlreadyPresent(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId)
    {
        auto &arrPos          = Registry::getInstance().getComponents<Types::Position>();
        auto &arrHealth       = Registry::getInstance().getComponents<struct health_s>();
        auto &arrOtherPlayers = Registry::getInstance().getComponents<Types::OtherPlayer>();
        auto ids              = Registry::getInstance().getEntitiesByComponents(
            {typeid(Types::Position), typeid(Types::OtherPlayer), typeid(struct health_s)});

        for (const auto &e : _playersIds) {
            auto allieId = e.second;
            if (allieId == playerId) {
                continue;
            }
            auto idIt = std::find_if(ids.begin(), ids.end(), [allieId, &arrOtherPlayers](std::size_t id) {
                return arrOtherPlayers[id].constId == allieId;
            });
            if (idIt == ids.end()) {
                continue;
            }
            struct packetCreatePlayer_s packetMsgCreatePlayer = {
                .header = {0, 0, 0, 0, 1, 0},
                .action = {.magick = NEW_PLAYER},
                .msg    = {
                           .magick        = MAGICK_NEW_PLAYER,
                           .playerId      = allieId,
                           .pos           = {arrPos[*idIt].x, arrPos[*idIt].y},
                           .life          = arrHealth[*idIt],
                           .isOtherPlayer = 1}
            };
            sendNewAllie(allieId, packetMsgCreatePlayer, endpoint, false);
        }
    }

    void
    NitworkServer::handleInitMsg(const std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        if (_endpoints.size() >= _serverInfos.maxNbPlayer) {
            Logger::error("Too many clients, can't add an other one");
            return;
        }
        if (isClientAlreadyConnected(endpoint)) {
            Logger::error("Client already connected");
            return;
        }
        _endpoints.emplace_back(endpoint);
        auto playerId      = static_cast<n_id_t>(_endpoints.size() - 1);
        auto &jsonInstance = Json::getInstance();

        struct packetCreatePlayer_s packetMsgCreatePlayer = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_PLAYER},
            .msg    = {
                       .magick   = MAGICK_NEW_PLAYER,
                       .playerId = playerId,
                       .pos =
                    {jsonInstance
                            .getDataByVector<int>({"player", "position", "x"}, JsonType::DEFAULT_PLAYER),
                        jsonInstance
                            .getDataByVector<int>({"player", "position", "y"}, JsonType::DEFAULT_PLAYER)},
                       .life = {jsonInstance.getDataByVector<int>({"player", "health"}, JsonType::DEFAULT_PLAYER)},
                       .isOtherPlayer = 0}
        };
        addPlayerInitMessage(endpoint, packetMsgCreatePlayer.msg);
        packetMsgCreatePlayer.msg.isOtherPlayer = 1;
        Systems::initPlayer(
            playerId,
            packetMsgCreatePlayer.msg.pos,
            packetMsgCreatePlayer.msg.life,
            static_cast<bool>(packetMsgCreatePlayer.msg.isOtherPlayer));
        sendNewAllie(playerId, packetMsgCreatePlayer, endpoint);
        sendAlliesAlreadyPresent(endpoint, playerId);
    }

    void
    NitworkServer::handleReadyMsg(const std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (!isClientAlreadyConnected(endpoint)) {
            Logger::info("Client not connected");
            return;
        }
        if (_endpoints.size() < _serverInfos.maxNbPlayer) {
            Logger::info("A new client is ready, waiting for others");
            return;
        }
        addStarWaveMessage(Types::Enemy::getEnemyNb());
        auto &director = Systems::SystemManagersDirector::getInstance();
        std::lock_guard<std::mutex> lock(director.mutex);
        director.getSystemManager(0).addSystem(Systems::initWave);
    }

    void
    NitworkServer::handleRelativePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (!isClientAlreadyConnected(endpoint)) {
            Logger::info("Client not connected");
            return;
        }
        auto msgData = std::any_cast<struct msgPositionRelative_s>(msg);
        auto pos     = msgData.pos;
        struct packetPositionRelativeBroadcast_s msgPosBroadcast = {
            .header =
                {.magick1          = HEADER_CODE1,
                         .ids_received     = 0,
                         .last_id_received = 0,
                         .id               = 0,
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2},
            .action = {.magick = POSITION_RELATIVE_BROADCAST},
            .msg    = {
                         .magick   = MAGICK_POSITION_RELATIVE_BROADCAST,
                         .pos      = {.x = pos.x, .y = pos.y},
                         .playerId = getPlayerId(endpoint)}
        };
        Packet packet(
            msgPosBroadcast.action.magick,
            std::make_any<struct packetPositionRelativeBroadcast_s>(msgPosBroadcast));
        sendToAllClientsButNotOne(packet, endpoint);
    }
    /* End Handle packet (msg) Section */

    /* Message Creation Section */
    void NitworkServer::addInfoLobbyMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetInfoLobby_s packetInfo = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = INFO_LOBBY},
            .msg    = {
                       .magick      = MAGICK_INFO_LOBBY,
                       .name        = {},
                       .maxNbPlayer = _serverInfos.maxNbPlayer,
                       .gameType    = _serverInfos.gameType,
                       .ownerInfos  = _serverInfos.ownerInfos}
        };
        std::strcpy(packetInfo.msg.name, _serverInfos.name);
        boost::asio::ip::udp::endpoint mainServer = boost::asio::ip::udp::endpoint(
            boost::asio::ip::address::from_string(_serverInfos.ownerInfos.ip),
            _serverInfos.ownerInfos.port);
        Packet packet(
            packetInfo.action.magick,
            std::make_any<struct packetInfoLobby_s>(packetInfo),
            mainServer);
        addPacketToSend(packet);
    }

    void NitworkServer::addPlayerInitMessage(
        boost::asio::ip::udp::endpoint &endpoint,
        const msgCreatePlayer_s &playerMsg)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetCreatePlayer_s packetCreatePlayer = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_PLAYER},
            .msg    = playerMsg
        };
        Packet packet(
            packetCreatePlayer.action.magick,
            std::make_any<struct packetCreatePlayer_s>(packetCreatePlayer),
            endpoint);
        addPacketToSend(packet);
        _playersIds[endpoint] = playerMsg.playerId;
    }

    void NitworkServer::addStarWaveMessage(n_id_t enemyNb)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgStartWave_s packetMsgStartWave = {
            .header       = {0, 0, 0, 0, 1, 0},
            .action       = {.magick = START_WAVE},
            .msgStartWave = {.magick = MAGICK_START_WAVE, .enemyNb = enemyNb}
        };
        Packet packet(
            packetMsgStartWave.action.magick,
            std::make_any<struct packetMsgStartWave_s>(packetMsgStartWave));
        sendToAllClients(packet);
    }

    void NitworkServer::addLifeUpdateMessage(
        boost::asio::ip::udp::endpoint &endpoint,
        n_id_t playerId,
        const struct health_s &life)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetLifeUpdate_s packetLifeUpdate = {
            .header        = {0, 0, 0, 0, 1, 0},
            .action        = {.magick = LIFE_UPDATE},
            .msgLifeUpdate = {.magick = MAGICK_LIFE_UPDATE, .playerId = playerId, .life = life}
        };
        Packet packet(
            packetLifeUpdate.action.magick,
            std::make_any<struct packetLifeUpdate_s>(packetLifeUpdate),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkServer::addEnemyDeathMessage(n_id_t enemyId)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetEnemyDeath_s packetEnemyDeath = {
            .header        = {0, 0, 0, 0, 1, 0},
            .action        = {.magick = ENEMY_DEATH},
            .msgEnemyDeath = {.magick = MAGICK_ENEMY_DEATH, .enemyId = {.id = enemyId}}
        };
        Packet packet(
            packetEnemyDeath.action.magick,
            std::make_any<struct packetEnemyDeath_s>(packetEnemyDeath));
        sendToAllClients(packet);
    }

    void NitworkServer::addNewEnemyMessage(
        boost::asio::ip::udp::endpoint &endpoint,
        const struct enemy_infos_s &enemyInfos)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetNewEnemy_s packetNewEnemy = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_ENEMY},
            .msg    = {.magick = MAGICK_NEW_ENEMY, .enemyInfos = enemyInfos}
        };
        Packet packet(
            packetNewEnemy.action.magick,
            std::make_any<struct packetNewEnemy_s>(packetNewEnemy),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkServer::broadcastNewBulletMsg(
        const struct msgNewBullet_s &msg)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetNewBullet_s packetNewBullet = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_MISSILE},
            .msg    = msg
        };
        Packet packet(
            packetNewBullet.action.magick,
            std::make_any<struct packetNewBullet_s>(packetNewBullet));
        sendToAllClients(packet);
    }

    void NitworkServer::broadcastAbsolutePositionMsg(
        const struct position_absolute_s &pos,
        boost::asio::ip::udp::endpoint &senderEndpoint)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPositionAbsoluteBroadcast_s packetPosAbsoluteBroadcast = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = POSITION_ABSOLUTE_BROADCAST},
            .msg    = {
                       .magick   = MAGICK_POSITION_ABSOLUTE_BROADCAST,
                       .pos      = {.x = pos.x, .y = pos.y},
                       .playerId = getPlayerId(senderEndpoint)}
        };
        Packet packet(
            packetPosAbsoluteBroadcast.action.magick,
            std::make_any<struct packetPositionAbsoluteBroadcast_s>(packetPosAbsoluteBroadcast));
        sendToAllClientsButNotOne(packet, senderEndpoint);
    }

    void NitworkServer::addPlayerDeathMsg(n_id_t id)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPlayerDeath_s packetPlayerDeath = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = PLAYER_DEATH},
            .msg    = {.magick = MAGICK_PLAYER_DEATH, .playerId = id}
        };
        Packet packet(
            packetPlayerDeath.action.magick,
            std::make_any<struct packetPlayerDeath_s>(packetPlayerDeath));
        sendToAllClients(packet);
    }

    void NitworkServer::addPlayerDeathMsg(n_id_t id, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPlayerDeath_s packetPlayerDeath = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = PLAYER_DEATH},
            .msg    = {.magick = MAGICK_PLAYER_DEATH, .playerId = id}
        };
        Packet packet(
            packetPlayerDeath.action.magick,
            std::make_any<struct packetPlayerDeath_s>(packetPlayerDeath),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkServer::addNewPlayerMsg(
        boost::asio::ip::udp::endpoint &endpoint,
        const struct msgCreatePlayer_s &playerMsg)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetCreatePlayer_s packetCreatePlayer = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_PLAYER},
            .msg    = playerMsg
        };
        Packet packet(
            packetCreatePlayer.action.magick,
            std::make_any<struct packetCreatePlayer_s>(packetCreatePlayer),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkServer::addMissileDeathMsg(n_id_t id)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMissileDeath_s packetMissileDeath = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = MISSILE_DEATH,
                },
            .msgMissileDeath =
                {
                       .magick  = MAGICK_MISSILE_DEATH,
                       .missileId = id,
                },
        };
        Packet packet(
            packetMissileDeath.action.magick,
            std::make_any<struct packetMissileDeath_s>(packetMissileDeath));
        sendToAllClients(packet);
    }

    n_id_t NitworkServer::getPlayerId(const boost::asio::ip::udp::endpoint &endpoint) const
    {
        return _playersIds.at(endpoint);
    }
} // namespace Nitwork
