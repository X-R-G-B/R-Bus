/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkServer
*/

#include "NitworkServer.hpp"

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

    bool NitworkServer::startNitworkConfig(int port, __attribute((unused)) const std::string &ip)
    {
        _endpoint = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
        _socket.open(boost::asio::ip::udp::v4());
        if (!_socket.is_open()) {
            std::cerr << "Error: socket not open" << std::endl;
            return false;
        }
        _socket.bind(_endpoint);
        return true;
    }

    void NitworkServer::handleBodyAction(__attribute__((unused))
                                         const boost::asio::ip::udp::endpoint &endpoint)
    {
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data() + sizeof(struct header_s));
        auto it      = _actionsHandlers.find(action->magick);

        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            return;
        }
        it->second.first(it->second.second);
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
    void NitworkServer::handleInitMsg(__attribute__((unused)) const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
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
        __attribute__((unused)) const std::any &msg,
        __attribute__((unused)) boost::asio::ip::udp::endpoint &endpoint)
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
            .header =
                {.magick1          = HEADER_CODE1,
                         .ids_received     = 0,
                         .last_id_received = (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
                         .id               = getPacketID(),
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2},
            .action       = {.magick = START_GAME},
            .msgStartGame = {.magick = MAGICK_START_GAME, .playerId = playerId}
        };
        struct packet_s packetData = {
            .action = packetMsgStartGame.action.magick,
            .body   = std::make_any<struct packetMsgStartGame_s>(packetMsgStartGame)};
        addPacketToSend(endpoint, packetData);
    }
} // namespace Nitwork
