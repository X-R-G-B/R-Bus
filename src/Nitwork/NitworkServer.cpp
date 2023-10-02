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
        return ANitwork::start(port, threadNb, tick);
    }

    bool NitworkServer::startNitworkConfig(int port, const std::string &ip /* unused */)
    {
        _endpoint =
            boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
        _socket.open(boost::asio::ip::udp::v4());
        if (!_socket.is_open()) {
            std::cerr << "Error: socket not open" << std::endl;
            return false;
        }
        _socket.bind(_endpoint);
        return true;
    }

    void NitworkServer::handleBodyAction(
        const struct header_s header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        auto *action = reinterpret_cast<struct action_s *>(
            _receiveBuffer.data() + sizeof(struct header_s));
        std::cout << "action.magick: " << action->magick << std::endl;
        auto endPointIt = std::find_if(
            _endpoints.begin(), _endpoints.end(),
            [&endpoint](const boost::asio::ip::udp::endpoint &e) {
                return e.address() == endpoint.address();
        });
        if (endPointIt == _endpoints.end() && action->magick != INIT) {
            std::cerr << "Error: endpoint not found" << std::endl;
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

    /* Getters Section */
    const std::map<
        enum n_actionType_t,
        actionHandler
    >&
    NitworkServer::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }
    /* End Getters Section */

    /* Handle packet (msg) Section */
    void NitworkServer::handleInitMsg(
        const std::any &msg,
        boost::asio::ip::udp::endpoint &endpoint)
    {
        const struct msgInit_s &initMsg = std::any_cast<struct msgInit_s>(msg);

        std::cout << "init" << std::endl;
        if (_endpoints.size() >= MAX_CLIENTS) {
            std::cerr << "Too many clients, can't add an other one" << std::endl;
            return;
        }
        auto endPointIt = std::find_if(
            _endpoints.begin(), _endpoints.end(),
            [&endpoint](const boost::asio::ip::udp::endpoint &e) {
                return e.address() == endpoint.address();
        });
        if (endPointIt != _endpoints.end()) {
            std::cerr << "Error: endpoint already init" << std::endl;
            return;
        }
        _endpoints.emplace_back(endpoint);
    }

    void NitworkServer::handleReadyMsg(
        const std::any &msg,
        boost::asio::ip::udp::endpoint &endpoint)
    {
        const struct msgReady_s &readyMsg =
            std::any_cast<struct msgReady_s>(msg);
        std::cout << "ready" << std::endl;
    }
    /* End Handle packet (msg) Section */
}
