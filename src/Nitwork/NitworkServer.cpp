/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkServer
*/

#include "NitworkServer.hpp"
#include "Logger.hpp"

namespace Nitwork {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    NitworkServer NitworkServer::_instance = NitworkServer();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

    NitworkServer &NitworkServer::getInstance()
    {
        return _instance;
    }

    bool NitworkServer::start(int port, int threadNb, int tick, const std::string &ip)
    {
        return ANitwork::start(port, threadNb, tick, ip);
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
            addPacketToSend(endpoint, packet);
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
        Logger::info(
            "Received packet from " + endpoint.address().to_string() + ":" + std::to_string(endpoint.port())
            + " with id " + std::to_string(header.id) + " and action of type "
            + std::to_string(action->magick));
        it->second.first(it->second.second, header);
    }

    /* Getters Section */
    const std::map<enum n_actionType_t, actionHandler> &NitworkServer::getActionToSendHandlers() const
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

    /* Handle packet (msg) Section */
    void
    NitworkServer::handleInitMsg(const std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (_endpoints.size() >= MAX_CLIENTS) {
            std::cerr << "Too many clients, can't add an other one" << std::endl;
            return;
        }
        if (isClientAlreadyConnected(endpoint)) {
            std::cerr << "Client already connected" << std::endl;
            return;
        }
        _endpoints.emplace_back(endpoint);
    }

    void NitworkServer::handleReadyMsg(
        const std::any & /* unused */,
        boost::asio::ip::udp::endpoint &endpoint /* unused */)
    {
        if (!isClientAlreadyConnected(endpoint)) {
            std::cerr << "Client not connected" << std::endl;
            return;
        }
    }
    /* End Handle packet (msg) Section */

    /* Message Creation Section */
    void NitworkServer::addStarGameMessage(boost::asio::ip::udp::endpoint &endpoint, n_id_t playerId)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgStartGame_s packetMsgStartGame = {
            {HEADER_CODE1,
             getIdsReceived(),
             (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
             getPacketID(),
             1, HEADER_CODE2},
            {START_GAME},
            {MAGICK_START_GAME,             playerId               }
        };
        Packet packet(
            packetMsgStartGame.header.id,
            packetMsgStartGame.action.magick,
            std::make_any<struct packetMsgStartGame_s>(packetMsgStartGame));
        std::cout << "Send START_GAME to " << endpoint.address().to_string() << ":" << endpoint.port()
                  << std::endl;
        addPacketToSend(endpoint, packet);
    }

    void NitworkServer::addLifeUpdateMessage(
        boost::asio::ip::udp::endpoint &endpoint,
        n_id_t playerId,
        const struct health_s &life)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetLifeUpdate_s packetLifeUpdate = {
            .header =
                {.magick1          = HEADER_CODE1,
                         .ids_received     = getIdsReceived(),
                         .last_id_received = (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
                         .id               = getPacketID(),
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2},
            .action        = {.magick = LIFE_UPDATE           },
            .msgLifeUpdate = {.magick = MAGICK_LIFE_UPDATE,                     .playerId = playerId,     .life = life                                                 }
        };
        Packet packet(
            packetLifeUpdate.header.id,
            packetLifeUpdate.action.magick,
            std::make_any<struct packetLifeUpdate_s>(packetLifeUpdate));
        addPacketToSend(endpoint, packet);
    }

    void NitworkServer::addEnemyDeathMessage(n_id_t enemyId)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetEnemyDeath_s packetEnemyDeath = {
            .header =
                {.magick1          = HEADER_CODE1,
                         .ids_received     = getIdsReceived(),
                         .last_id_received = (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
                         .id               = getPacketID(),
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2},
            .action        = {.magick = ENEMY_DEATH},
            .msgEnemyDeath = {.magick = MAGICK_ENEMY_DEATH,                                 .enemyId = {.id = enemyId}               }
        };
        Packet packet(
            packetEnemyDeath.header.id,
            packetEnemyDeath.action.magick,
            std::make_any<struct packetEnemyDeath_s>(packetEnemyDeath));
        sendToAllClients(packet);
    }
} // namespace Nitwork
