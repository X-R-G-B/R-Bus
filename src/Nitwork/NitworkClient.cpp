/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** NitworkClient
*/

#include "NitworkClient.hpp"
#include "Registry.hpp"

namespace Nitwork {
    NitworkClient::NitworkClient() : _resolver(_context)
    {
    }

    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    NitworkClient NitworkClient::_instance = NitworkClient();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    NitworkClient &NitworkClient::getInstance()
    {
        return _instance;
    }

    bool NitworkClient::start(int port, int threadNb, int tick, const std::string &ip)
    {
        return ANitwork::start(port, threadNb, tick, ip);
    }

    bool NitworkClient::startNitworkConfig(int port, const std::string &ip)
    {
        std::lock_guard<std::mutex> lock(Registry::getInstance().mutex);
        _endpoints.emplace_back(
            *_resolver.resolve(boost::asio::ip::udp::v4(), ip, std::to_string(port)).begin());
        _serverEndpoint = _endpoints.back();
        _socket         = boost::asio::ip::udp::socket(_context);
        _socket.open(boost::asio::ip::udp::v4());
        _socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));
        if (!_socket.is_open()) {
            Registry::getInstance().getLogger().error("Socket not open");
            return false;
        }
        return true;
    }

    void NitworkClient::handleBodyAction(
        const struct header_s &header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data() + sizeof(struct header_s));
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (endpoint.address().to_string() != _serverEndpoint.address().to_string()) {
            Logger::error(
                "NITWORK: endpoint " + endpoint.address().to_string() + " is not the server"
                + _serverEndpoint.address().to_string());
            return;
        }
        auto it = _actionsHandlers.find(action->magick);
        if (it == _actionsHandlers.end()) {
            Logger::error("NITWORK: action not found");
            return;
        }
        Logger::debug(
            "Received packet from " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port())
            + " with id " + std::to_string(header.id) + " and action of type "
            + std::to_string(action->magick));
        it->second.first(it->second.second, header);
    }

    const std::map<enum n_actionType_t, actionSender> &NitworkClient::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }

    /* Message Creation Section */
    void NitworkClient::addInitMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgInit_s packetMsgInit = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = INIT,
                       },
            .msgInit = {.magick = MAGICK_INIT}
        };
        Packet packet(
            packetMsgInit.action.magick,
            std::make_any<struct packetMsgInit_s>(packetMsgInit),
            _serverEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::addReadyMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgReady_s packetMsgReady = {
            .header   = {0, 0, 0, 0, 1, 0},
            .action   = {.magick = READY},
            .msgReady = {.magick = MAGICK_READY}
        };
        Packet packet(
            packetMsgReady.action.magick,
            std::make_any<struct packetMsgReady_s>(packetMsgReady),
            _serverEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::addPositionRelativeMsg(struct position_relative_s pos)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPositionRelative_s packetMsgPositionRelative = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = POSITION_RELATIVE,
                       },
            .msg =
                {
                       .magick = MAGICK_POSITION_RELATIVE,
                       .pos    = pos,
                       },
        };
        Packet packet(
            packetMsgPositionRelative.action.magick,
            std::make_any<struct packetPositionRelative_s>(packetMsgPositionRelative),
            _serverEndpoint);

        addPacketToSend(packet);
    }

    void
    NitworkClient::addNewBulletMsg(const struct position_absolute_s &pos, const missileTypes_e &missileType)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetNewBullet_s packetNewBullet = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = NEW_BULLET,
                       },
            .msg =
                {
                       .magick      = MAGICK_NEW_BULLET,
                       .pos         = pos,
                       .missileType = missileType,
                       },
        };
        Packet packet(
            packetNewBullet.action.magick,
            std::make_any<struct packetNewBullet_s>(packetNewBullet),
            _serverEndpoint);
        addPacketToSend(packet);
    };

    void NitworkClient::addPositionAbsoluteMsg(struct position_absolute_s pos)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPositionAbsolute_s packetMsgPositionAbsolute = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = POSITION_ABSOLUTE,
                       },
            .msg =
                {
                       .magick = MAGICK_POSITION_ABSOLUTE,
                       .pos    = pos,
                       },
        };
        Packet packet(
            packetMsgPositionAbsolute.action.magick,
            std::make_any<struct packetPositionAbsolute_s>(packetMsgPositionAbsolute),
            _serverEndpoint);

        addPacketToSend(packet);
    }

    void NitworkClient::addLifeUpdateMsg(n_id_t playerId, const struct health_s &life)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetLifeUpdate_s packetLifeUpdate = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = LIFE_UPDATE,
                       },
            .msgLifeUpdate =
                {
                       .magick   = MAGICK_LIFE_UPDATE,
                       .playerId = playerId,
                       .life     = life,
                       },
        };
        Packet packet(
            packetLifeUpdate.action.magick,
            std::make_any<struct packetLifeUpdate_s>(packetLifeUpdate),
            _serverEndpoint);

        addPacketToSend(packet);
    }

    void NitworkClient::addEnemyDeathMsg(n_id_t id)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetEnemyDeath_s packetEnemyDeath = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = ENEMY_DEATH,
                       },
            .msgEnemyDeath =
                {
                       .magick  = MAGICK_ENEMY_DEATH,
                       .enemyId = {.id = id},
                       },
        };
        Packet packet(
            packetEnemyDeath.action.magick,
            std::make_any<struct packetEnemyDeath_s>(packetEnemyDeath),
            _serverEndpoint);
        addPacketToSend(packet);
    }
} // namespace Nitwork
