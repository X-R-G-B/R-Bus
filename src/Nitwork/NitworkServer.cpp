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

    bool NitworkServer::startServer(int port, int nbPlayer, int threadNb, int tick)
    {
        _maxNbPlayer = nbPlayer;
        return ANitwork::start(port, threadNb, tick, "");
    }

    bool NitworkServer::startNitworkConfig(int port, const std::string & /* unused */)
    {
        boost::asio::ip::udp::endpoint endpoint =
            boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), boost::asio::ip::port_type(port));

        _socket.open(boost::asio::ip::udp::v4());
        if (!_socket.is_open()) {
            std::cerr << "Error: socket not open" << std::endl;
            return false;
        }
        _socket.bind(endpoint);
        return true;
    }

    void NitworkServer::sendToAllClients(const Packet &packet)
    {
        for (auto &endpoint : _endpoints) {
            addPacketToSend(Packet(packet, endpoint));
        }
    }

    void
    NitworkServer::sendToAllClientsButNotOne(const Packet &packet, boost::asio::ip::udp::endpoint &endpoint)
    {
        for (auto &e : _endpoints) {
            if (e != endpoint) {
                Logger::debug(
                    "Package sent to: " + e.address().to_string() + ":" + std::to_string(e.port()));
                addPacketToSend(Packet(packet, e));
            }
        }
    }

    void NitworkServer::handleBodyAction(
        const struct header_s &header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data() + sizeof(struct header_s));
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto it = _actionsHandlers.find(action->magick);

        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            return;
        }
        if (it->second.first == nullptr) {
            std::cerr << "Error: action handler is null" << std::endl;
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

    /* Check Methods Section */
    bool NitworkServer::isClientAlreadyConnected(boost::asio::ip::udp::endpoint &endpoint) const
    {
        auto endPointIt = std::find_if(
            _endpoints.begin(),
            _endpoints.end(),
            [&endpoint](const boost::asio::ip::udp::endpoint &e) {
                return e.address() == endpoint.address() && e.port() == endpoint.port();
            });

        return endPointIt != _endpoints.end();
    }
    /* End Check Methods Section */

    void NitworkServer::sendNewAllie(n_id_t playerId, struct packetNewAllie_s packetMsgNewAllie, boost::asio::ip::udp::endpoint &endpoint, bool butNoOne)
    {
        packetMsgNewAllie.msg.playerId = playerId;
        if (butNoOne) {
            Packet packet(
                packetMsgNewAllie.action.magick,
                std::make_any<struct packetNewAllie_s>(packetMsgNewAllie));
            sendToAllClientsButNotOne(packet, endpoint);
        } else {
            Packet packet(
                packetMsgNewAllie.action.magick,
                std::make_any<struct packetNewAllie_s>(packetMsgNewAllie),
                endpoint);
            addPacketToSend(packet);
        }
    }

    /* Handle packet (msg) Section */
    void
    NitworkServer::handleInitMsg(const std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (_endpoints.size() >= _maxNbPlayer) {
            std::cerr << "Too many clients, can't add an other one" << std::endl;
            return;
        }
        if (isClientAlreadyConnected(endpoint)) {
            std::cerr << "Client already connected" << std::endl;
            return;
        }
        _endpoints.emplace_back(endpoint);
        auto playerId = static_cast<n_id_t>(_endpoints.size() - 1);
        // Send new Allie to others
        addPlayerInitMessage(endpoint, playerId);
        struct packetNewAllie_s packetMsgNewAllie = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_ALLIE},
            .msg    = {.magick = MAGICK_NEW_ALLIE, .playerId = playerId}
        };
        Systems::initPlayer(JsonType::DEFAULT_PLAYER, playerId, true);
        Logger::info("before sendNewAllie");
        sendNewAllie(playerId, packetMsgNewAllie, endpoint);
        Logger::info("after sendNewAllie");
        for (const auto &[_, allieId] : _playersIds) {
            if (allieId == playerId) {
                continue;
            }
            Logger::info("loop");
            sendNewAllie(allieId, packetMsgNewAllie, endpoint, false);
        }
    }

    void
    NitworkServer::handleReadyMsg(const std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (!isClientAlreadyConnected(endpoint)) {
            Logger::info("Client not connected");
            return;
        }
        if (_endpoints.size() < _maxNbPlayer) {
            Logger::info("A new client is ready, waiting for others");
            return;
        }
        addStarWaveMessage(endpoint, Types::Enemy::getEnemyNb());
        Systems::SystemManagersDirector::getInstance().getSystemManager(0).addSystem(Systems::initWave);
    }

    void
    NitworkServer::handleRelativePositionMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (!isClientAlreadyConnected(endpoint)) {
            Logger::info("Client not connected");
            return;
        }
        auto msgData = std::any_cast<struct msgPositionRelative_s>(msg);
        auto pos = msgData.pos;
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
    void NitworkServer::addPlayerInitMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgPlayerInit_s packetMsgPlayerInit = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = INIT},
            .msg    = {.magick = MAGICK_INIT, .playerId = playerId}
        };
        Packet packet(
            packetMsgPlayerInit.action.magick,
            std::make_any<struct packetMsgPlayerInit_s>(packetMsgPlayerInit),
            endpoint);
        addPacketToSend(packet);
        _playersIds[endpoint] = playerId;
    }

    void NitworkServer::addStarWaveMessage(boost::asio::ip::udp::endpoint & /* unused */, n_id_t enemyId)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgStartWave_s packetMsgStartWave = {
            .header       = {0, 0, 0, 0, 1, 0},
            .action       = {.magick = START_WAVE},
            .msgStartWave = {.magick = MAGICK_START_WAVE, .enemyNb = enemyId}
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
        const struct msgNewBullet_s &msg,
        boost::asio::ip::udp::endpoint &senderEndpoint)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetNewBullet_s packetNewBullet = {
            .header = {0, 0, 0, 0, 1, 0},
            .action = {.magick = NEW_BULLET},
            .msg    = msg
        };
        Packet packet(
            packetNewBullet.action.magick,
            std::make_any<struct packetNewBullet_s>(packetNewBullet));
        sendToAllClientsButNotOne(packet, senderEndpoint);
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

    n_id_t NitworkServer::getPlayerId(const boost::asio::ip::udp::endpoint &endpoint) const
    {
        return _playersIds.at(endpoint);
    }
} // namespace Nitwork
