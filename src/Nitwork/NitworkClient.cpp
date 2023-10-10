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
        _endpoint = *_resolver.resolve(boost::asio::ip::udp::v4(), ip, std::to_string(port)).begin();
        boost::asio::ip::udp::socket socket(_context);
        _socket.open(boost::asio::ip::udp::v4());
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
        if (endpoint.address().to_string() != _endpoint.address().to_string()) {
            std::cerr << "Error: endpoint is not the server" << std::endl;
            return;
        }
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

    const std::map<enum n_actionType_t, actionHandler> &NitworkClient::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }

    /* Handlers Section */
    // NOLINTBEGIN(readability-convert-member-functions-to-static)
    void NitworkClient::handleStartGame(const std::any &msg, boost::asio::ip::udp::endpoint & /* unused */)
    {
        // NOLINTEND(readability-convert-member-functions-to-static)
        const struct msgStartGame_s &msgStartGame = std::any_cast<struct msgStartGame_s>(msg);

        if (msgStartGame.magick != MAGICK_START_GAME) {
            std::cerr << "Error: magick is not START_GAME" << std::endl;
            return;
        }
        std::cout << "Game started" << std::endl;
    }
    /* End Handlers Section */

    /* Message Creation Section */
    void NitworkClient::addInitMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgInit_s packetMsgInit = {
            {
             HEADER_CODE1,                                                    getIdsReceived(),
             (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
             getPacketID(),
             1, HEADER_CODE2,
             },
            {INIT},
            {MAGICK_INIT                                                              }
        };
        Packet packet(
            packetMsgInit.header.id,
            packetMsgInit.action.magick,
            std::make_any<struct packetMsgInit_s>(packetMsgInit));
        addPacketToSend(_endpoint, packet);
    }

    void NitworkClient::addReadyMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgReady_s packetMsgReady = {
            {
             HEADER_CODE1,                                                    getIdsReceived(),
             (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
             getPacketID(),
             1, HEADER_CODE2,
             },
            {READY},
            {MAGICK_READY                                                              }
        };
        Packet packet(
            packetMsgReady.header.id,
            packetMsgReady.action.magick,
            std::make_any<struct packetMsgReady_s>(packetMsgReady));
        addPacketToSend(_endpoint, packet);
    }

    void NitworkClient::addPositionRelativeMsg(struct position_relative_s pos)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPositionRelative_s packetMsgPositionRelative = {
            .header =
                {
                         .magick1          = HEADER_CODE1,
                         .ids_received     = getIdsReceived(),
                         .last_id_received = (!_receivedPacketsIds.empty()) ? _receivedPacketsIds.back() : 0,
                         .id               = getPacketID(),
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2,
                         },
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
            packetMsgPositionRelative.header.id,
            packetMsgPositionRelative.action.magick,
            std::make_any<struct packetPositionRelative_s>(packetMsgPositionRelative));

        addPacketToSend(_endpoint, packet);
    }
} // namespace Nitwork
