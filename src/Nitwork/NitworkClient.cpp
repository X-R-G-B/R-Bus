/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** NitworkClient
*/

#include "NitworkClient.hpp"

namespace Nitwork {
    NitworkClient::NitworkClient()
        : _resolver(_context) {}

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
            std::cerr << "Error: socket not open" << std::endl;
            return false;
        }
        return true;
    }

    void NitworkClient::handleBodyAction(
        const struct header_s header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        auto *action = reinterpret_cast<struct action_s *>(
            _receiveBuffer.data() + sizeof(struct header_s));
        std::cout << "action.magick: " << action->magick << std::endl;
        if (endpoint.address().to_string() != _endpoint.address().to_string()) {
            std::cerr << "Error: endpoint is not the server" << std::endl;
            startReceiveHandler();
            return;
        }
        auto it = _actionsHandlers.find(action->magick);
        if (it == _actionsHandlers.end()) {
            std::cerr << "Error: action not found" << std::endl;
            startReceiveHandler();
            return;
        }
        it->second.first(header, it->second.second);
        startReceiveHandler();
    }

    const std::map<enum n_actionType_t, actionHandler> &NitworkClient::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }

    void NitworkClient::handleInitMsg(
        const std::any &msg,
        boost::asio::ip::udp::endpoint &endpoint)
    {
        std::cout << "init meme si le client en a pas ???" << std::endl;
    }

    void NitworkClient::handleReadyMsg(const std::any &msg, boost::asio::ip::udp::endpoint &endpoint)
    {
        std::cout << "ready meme si le client en a pas ???" << std::endl;
    }

    /* Getters Section */
    n_idsReceived_t NitworkClient::getIdsReceived()
    {
        n_idsReceived_t idsReceived = 0;

        for (auto &id : _receivedPacketsIds) {
            idsReceived |= id;
        }
        return idsReceived;
    }


    /* Message Creation Section */
    void NitworkClient::addInitMsg() {
        std::cout << "Adding init msg" << std::endl;
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetMsgInit_s packetMsgInit = {
            .header = {
                .magick1 = HEADER_CODE1,
                .ids_received = getIdsReceived(),
                .last_id_received = (_receivedPacketsIds.size() > 0) ? _receivedPacketsIds.back() : 0,
                .id = getPacketID(),
                .nb_action = 1,
                .magick2 = HEADER_CODE2,
            },
            .action = {
                .magick = INIT
            },
            .msgInit = {
                .magick = INIT
            }
        };
        struct packet_s packetData = {
            .action = packetMsgInit.action.magick,
            .body = std::make_any<struct packetMsgInit_s>(packetMsgInit)
        };
        addPacketToSend(_endpoint, packetData);
    }
} // namespace Nitwork
