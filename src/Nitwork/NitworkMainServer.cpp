/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** NitworkMainServer
*/

extern "C"
{
#include <unistd.h>
}
#include "NitworkMainServer.hpp"
#include "NitworkServer.hpp"
#include "ResourcesManager.hpp"

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

    void NitworkMainServer::stop()
    {
        for (auto pid : _lobbyPids) {
            kill(pid, SIGINT);
        }
        ANitwork::stop();
    }

    /* Handlers methods of the received actions */
    void NitworkMainServer::sendOkServer(const boost::asio::ip::udp::endpoint &endpoint)
    {
        struct packetConnectMainServerResp_s packetConnectMainServerResp = {
            .header =
                {.magick1          = HEADER_CODE1,
                         .ids_received     = 0,
                         .last_id_received = 0,
                         .id               = 0,
                         .nb_action        = 1,
                         .magick2          = HEADER_CODE2},
            .action = {.magick = OK_SERVER},
            .msg    = {.magick = MAGICK_MSGCONNECTMAINSERVERRESP}
        };

        Packet packet(
            packetConnectMainServerResp.action.magick,
            std::make_any<struct packetConnectMainServerResp_s>(packetConnectMainServerResp),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkMainServer::handleInitMsg(std::any & /* unused */, boost::asio::ip::udp::endpoint &endpoint)
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
        sendOkServer(endpoint);
    }

    /* End of handlers methods of the received actions */

    bool NitworkMainServer::startNitworkConfig(int port, const std::string & /* unused */)
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
    void NitworkMainServer::sendListLobby(
        const boost::asio::ip::udp::endpoint &endpoint,
        const std::vector<struct lobby_s> &lobbies)
    {
        if (lobbies.empty()) {
            Logger::error("Lobbies is empty, can't send it");
            return;
        }
        struct packetListLobby_s msg = {
            .header      = {0, 0, 0, 0, static_cast<unsigned char>(lobbies.size()), 0},
            .actionLobby = {}
        };

        for (std::size_t i = 0; i < lobbies.size(); i++) {
            msg.actionLobby[i] = {
                .action = {.magick = LIST_LOBBY},
                .lobby  = {.magick = MAGICK_LIST_LOBBY, .lobby = lobbies[i]}
            };
        }
        Packet packet(
            msg.actionLobby[0].action.magick,
            std::make_any<struct packetListLobby_s>(msg),
            endpoint);
        addPacketToSend(packet);
    }

    void NitworkMainServer::forkProcessAndCreateLobby(
        unsigned int maxNbPlayer,
        const std::string &name,
        const std::string &ownerIp,
        int ownerPort)
    {
        pid_t c_pid = fork();

        if (c_pid == -1) {
            Logger::error("Error: fork failed");
            return;
        }
        if (c_pid == 0) {
            if (execl(
                    ECS::ResourcesManager::convertPath("./r-type_server").c_str(),
                    ECS::ResourcesManager::convertPath("./r-type_server").c_str(),
                    "1",
                    std::to_string(maxNbPlayer).c_str(),
                    name.c_str(),
                    ownerIp.c_str(),
                    std::to_string(ownerPort).c_str(),
                    NULL)) {
                Logger::error("Error: execl failed");
            }
        } else {
            _lobbyPids.emplace_back(c_pid);
        }
    }

    void NitworkMainServer::createLobby(unsigned int maxNbPlayer, const std::string &name)
    {
        if (maxNbPlayer < 1) {
            Logger::error("Invalid number of players: " + std::to_string(maxNbPlayer));
            return;
        }
        if (name.empty()) {
            Logger::error("Invalid name: " + name);
            return;
        }
        forkProcessAndCreateLobby(
            maxNbPlayer,
            name,
            _endpoints.back().address().to_string(),
            _endpoints.back().port());
    }

    const std::vector<struct lobby_s> &NitworkMainServer::getLobbies() const
    {
        return _lobbies;
    }

    void NitworkMainServer::addLobby(const struct lobby_s &lobby)
    {
        if (lobby.name[0] == '\0') {
            Logger::error("Invalid lobby name");
            return;
        }
        if (lobby.maxNbPlayer < 1) {
            Logger::error("Invalid lobby maxNbPlayer");
            return;
        }
        if (lobby.ownerInfos.ip[0] == '\0') {
            Logger::error("Invalid lobby owner ip");
            return;
        }
        if (lobby.lobbyInfos.ip[0] == '\0') {
            Logger::error("Invalid lobby ip");
            return;
        }
        _lobbies.push_back(lobby);
    }
} // namespace Nitwork
