/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkMainServer
*/

#include "NitworkServer.hpp"
#include "NitworkMainServer.hpp"

namespace Nitwork {
    // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
    NitworkMainServer NitworkMainServer::_instance = NitworkMainServer();
    // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)
    NitworkMainServer &NitworkMainServer::getInstance()
    {
        return _instance;
    }

    bool NitworkMainServer::startServer(int port, int nbPlayer, int threadNb, int tick)
    {
        _maxNbPlayer = nbPlayer;
        return ANitwork::start(port, threadNb, tick);
    }

    /* Handlers methods of the received actions */
    void NitworkMainServer::handleInitMsg(std::any &/* unused */, boost::asio::ip::udp::endpoint &endpoint)
    {
        if (_endpoints.size() >= _maxNbPlayer) {
            Logger::error("Too many clients, can't add an other one");
            return;
        }
        if (isClientAlreadyConnected(endpoint)) {
            Logger::error("Client already connected");
            return;
        }
        _endpoints.emplace_back(endpoint);
        // send Init message to the client, in order to let him know his connected
//        addInitMessage(endpoint);
    }

    /* End of handlers methods of the received actions */

    bool NitworkMainServer::startNitworkConfig(int port, const std::string &/* unused */)
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

    const std::map<enum n_actionType_t, actionSender> &NitworkMainServer::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }

    void NitworkMainServer::handleBodyAction(
        const struct header_s &header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data() + HEADER_SIZE);
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto it = _actionsHandlers.find(action->magick);

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

    /* Send methods */
    void NitworkMainServer::sendListLobby(const boost::asio::ip::udp::endpoint &endpoint, const std::vector<struct lobby_s> &lobbies)
    {
        if (lobbies.empty()) {
            Logger::error("Lobbies is empty, can't send it");
            return;
        }
        struct packetListLobby_s msg = {
            .header = {0, 0, 0, 0, static_cast<unsigned char>(lobbies.size()), 0},
            .actionLobby = {}
        };

        for (std::size_t i = 0; i < lobbies.size(); i++) {
            msg.actionLobby[i] = {
                .action = {.magick = LIST_LOBBY},
                .lobby = {
                           .magick = MAGICK_LIST_LOBBY,
                           .lobby = lobbies[i]
                }
            };
        }
        Packet packet(
            msg.actionLobby[0].action.magick,
            std::make_any<struct packetListLobby_s>(msg),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkMainServer::createLobby(const struct lobby_s &lobby)
    {
        std::thread([&lobby]() {
            // call ECS::ResourcesManager::init( with argv[0], for the path of the executable
            NitworkServer::getInstance().startServer(0, lobby.maxNbPlayer, DEFAULT_THREAD_NB, TICKS);
            auto serverInfos = NitworkServer::getInstance().getServerInfos();
            Logger::info("Server started on " + std::string(serverInfos.lobbyInfos.ip) + ":" + std::to_string(serverInfos.lobbyInfos.port));
        }).detach();
    }

    void NitworkMainServer::sendCreateLobby(const boost::asio::ip::udp::endpoint &endpoint, const struct lobby_s &lobby)
    {
        struct packetListLobby_s msg = {
            .header = {0, 0, 0, 0, 1, 0},
            .actionLobby = {}
        };

        //make detached thread
        createLobby(lobby);
        msg.actionLobby[0] = {
            .action = {.magick = LIST_LOBBY},
            .lobby = {
                       .magick = MAGICK_LIST_LOBBY,
                       .lobby = lobby
            }
        };
        Packet packet(
            msg.actionLobby[0].action.magick,
            std::make_any<struct packetListLobby_s>(msg),
            endpoint);
        addPacketToSend(packet);
    }
}