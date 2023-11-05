/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** NitworkClient
*/

#if defined(_WIN32)
    #define _CRT_SECURE_NO_WARNINGS
#endif
#include "B-luga/Logger.hpp"
#include "NitworkClient.hpp"
#include "B-luga/Registry.hpp"
#include "ResourcesManager.hpp"

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

    bool NitworkClient::startClient(int threadNb, int tick)
    {
        return ANitwork::start(0, threadNb, tick, "0.0.0.0");
    }

    bool NitworkClient::startNitworkConfig(int /* unused */, const std::string & /* unused */)
    {
        _socket = boost::asio::ip::udp::socket(_context);
        _socket.open(boost::asio::ip::udp::v4());
        _socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 0));
        if (!_socket.is_open()) {
            Logger::error("Socket not open");
            return false;
        }
        return true;
    }

    void NitworkClient::setMainEndpoint(const std::string &ip, n_port_t port)
    {
        if (ip.empty() || port == 0) {
            Logger::error("NITWORK: invalid main server endpoint");
            return;
        }
        removeEndpoint(_mainServerEndpoint.address().to_string(), _mainServerEndpoint.port());
        _mainServerEndpoint = addEndpoint(ip, port);
    }

    void NitworkClient::setLobbyEndpoint(const std::string &ip, n_port_t port)
    {
        if (ip.empty() || port == 0) {
            Logger::error("NITWORK: invalid server endpoint");
            return;
        }
        removeEndpoint(_serverEndpoint.address().to_string(), _serverEndpoint.port());
        _serverEndpoint = addEndpoint(ip, port);
    }

    boost::asio::ip::udp::endpoint &NitworkClient::addEndpoint(const std::string &ip, n_port_t port)
    {
        _endpoints.emplace_back(
            *_resolver.resolve(boost::asio::ip::udp::v4(), ip, std::to_string(port)).begin());
        return _endpoints.back();
    }

    void NitworkClient::removeEndpoint(const std::string &ip, n_port_t port)
    {
        for (auto it = _endpoints.begin(); it != _endpoints.end(); ++it) {
            if (it->address().to_string() == ip && it->port() == port) {
                _endpoints.erase(it);
                return;
            }
        }
    }

    void NitworkClient::handleBodyAction(
        const struct header_s &header,
        const boost::asio::ip::udp::endpoint &endpoint)
    {
        // NOLINTBEGIN(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        auto *action = reinterpret_cast<struct action_s *>(_receiveBuffer.data());
        // NOLINTEND(cppcoreguidelines-pro-type-reinterpret-cast,
        // cppcoreguidelines-pro-bounds-pointer-arithmetic)
        if (endpoint.address().to_string() != _serverEndpoint.address().to_string()
            && endpoint.address().to_string() != _mainServerEndpoint.address().to_string()) {
            Logger::error(
                "NITWORK: endpoint " + endpoint.address().to_string() + " is not the server"
                + _serverEndpoint.address().to_string() + " or the main server "
                + _mainServerEndpoint.address().to_string());
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
        std::memmove(
            _receiveBuffer.data(),
            _receiveBuffer.data() + sizeof(struct action_s),
            _receiveBuffer.size() - sizeof(struct action_s));
        std::memset(
            _receiveBuffer.data() + _receiveBuffer.size() - sizeof(struct action_s),
            0,
            sizeof(struct action_s));
        it->second.first(it->second.second, header);
    }

    const std::map<enum n_actionType_t, actionSender> &NitworkClient::getActionToSendHandlers() const
    {
        return _actionToSendHandlers;
    }

    /* Message Creation Section */
    bool NitworkClient::connectMainServer(const std::string &ip, n_port_t port)
    {
        try {
            setMainEndpoint(ip, port);
            addConnectMainServerMsg();
        } catch (const std::exception &e) {
            Logger::error("Failed to connect to main server: " + std::string(e.what()));
            return false;
        }
        return true;
    }

    void NitworkClient::connectLobby(const std::string &ip, n_port_t port)
    {
        setLobbyEndpoint(ip, port);
        addConnectLobbyMsg();
    }

    void NitworkClient::disconnectLobby()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetDisconnectLobby_s packetDisconnectLobby = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = DISCONNECT_LOBBY,
                       },
            .msg = {.magick = MAGICK_DISCONNECT_LOBBY},
        };
        Packet packet(
            packetDisconnectLobby.action.magick,
            std::make_any<struct packetDisconnectLobby_s>(packetDisconnectLobby),
            _serverEndpoint);
        Logger::fatal("Disconnecting from lobby");
        addPacketToSend(packet);
    }

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
                       .magick = NEW_MISSILE,
                       },
            .msg =
                {
                       .magick      = MAGICK_NEW_MISSILE,
                       .pos         = pos,
                       .id          = 0,
                       .life        = 0,
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

    void NitworkClient::addPlayerDeathMsg(n_id_t id)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetPlayerDeath_s packetPlayerDeath = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = PLAYER_DEATH,
                       },
            .msg = {.magick = MAGICK_PLAYER_DEATH, .playerId = id},
        };
        Packet packet(
            packetPlayerDeath.action.magick,
            std::make_any<struct packetPlayerDeath_s>(packetPlayerDeath),
            _serverEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::addMissileDeathMsg(n_id_t id)
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
                       .magick    = MAGICK_MISSILE_DEATH,
                       .missileId = id,
                       },
        };
        Packet packet(
            packetMissileDeath.action.magick,
            std::make_any<struct packetMissileDeath_s>(packetMissileDeath),
            _serverEndpoint);
    }

    void NitworkClient::addListLobbyMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetRequestListLobby_s packetListLobby = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = LIST_LOBBY,
                       },
            .msg = {.magick = MAGICK_REQUEST_LIST_LOBBY},
        };
        Packet packet(
            packetListLobby.action.magick,
            std::make_any<struct packetRequestListLobby_s>(packetListLobby),
            _mainServerEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::addCreateLobbyMsg(
        const std::string &name,
        enum gameType_e gameType,
        unsigned int maxNbPlayer)
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetCreateLobby_s packetCreateLobby = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = CREATE_LOBBY,
                       },
            .msg = {
                       .magick      = MAGICK_CREATE_LOBBY,
                       .name        = "",
                       .gameType    = gameType,
                       .maxNbPlayer = maxNbPlayer,
                       .ownerInfos  = {.ip = "", .port = _mainServerEndpoint.port()}}
        };
        std::strcpy(packetCreateLobby.msg.name, (name.size() > 32 ? name.substr(0, 32) : name).c_str());
        std::strcpy(
            packetCreateLobby.msg.ownerInfos.ip,
            (_mainServerEndpoint.address().to_string().size() > 16
                 ? _mainServerEndpoint.address().to_string().substr(0, 16)
                 : _mainServerEndpoint.address().to_string())
                .c_str());
        Packet packet(
            packetCreateLobby.action.magick,
            std::make_any<struct packetCreateLobby_s>(packetCreateLobby),
            _mainServerEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::addConnectMainServerMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetConnectMainServer_s packetConnectMainServer = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = CONNECT_MAIN_SERVER,
                       },
            .msg = {.magick = MAGICK_CONNECT_MAIN_SERVER},
        };
        Packet packet(
            packetConnectMainServer.action.magick,
            std::make_any<struct packetConnectMainServer_s>(packetConnectMainServer),
            _mainServerEndpoint);
        addPacketToSend(packet);
    }

    void NitworkClient::createForkedServer(const std::string &port)
        {
#ifdef _WIN32
        std::string winName = "'" + name + "'";
        std::basic_ostringstream<TCHAR> cmdline;
        cmdline << _T(ResourcesManager::convertPath("./r-type_server.exe").c_str()) << _T(" 0 ")
                << _T(" ") << _T(port);

        Logger::fatal("cmdline: " + cmdline.str());
        STARTUPINFO si = {sizeof(si)};
        PROCESS_INFORMATION pi;
    #ifdef UNICODE
        TCHAR *cmd = _wcsdup(cmdline.str().c_str());
    #else
        TCHAR *cmd = _strdup(cmdline.str().c_str());
    #endif

        if (!CreateProcess(NULL, cmd, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            Logger::error("Error: CreateProcess failed");
            free(cmd);
            return;
        }
        free(cmd);
        _serverPids.emplace_back(pi.dwProcessId);
#else
        pid_t c_pid = fork();

        if (c_pid == -1) {
            Logger::error("Error: fork failed");
            return;
        }
        if (c_pid == 0) {
            if (execl(
                    ResourcesManager::convertPath("r-type_server").c_str(),
                    ResourcesManager::convertPath("r-type_server").c_str(),
                    "0",
                    port.c_str(),
                    NULL)
                    == -1) {
                Logger::error("Error: execl failed");
                return;
            }
            Logger::info("Server created");
        } else {
            _serverPids.emplace_back(c_pid);
        }
#endif
    }

    void NitworkClient::stop()
    {
        #ifdef _WIN32
        for (auto pid : _serverPids) {
            HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
            if (hProcess != NULL) {
                TerminateProcess(hProcess, 0);
                CloseHandle(hProcess);
            }
        }
#else
        for (auto pid : _serverPids) {
            kill(pid, SIGINT);
        }
#endif
        ANitwork::stop();
    }

    void NitworkClient::addConnectLobbyMsg()
    {
        std::lock_guard<std::mutex> lock(_receivedPacketsIdsMutex);
        struct packetConnectLobby_s packetConnectLobby = {
            .header = {0, 0, 0, 0, 1, 0},
            .action =
                {
                       .magick = CONNECT_LOBBY,
                       },
            .msg = {.magick = MAGICK_CONNECT_LOBBY},
        };
        Packet packet(
            packetConnectLobby.action.magick,
            std::make_any<struct packetConnectLobby_s>(packetConnectLobby),
            _serverEndpoint);
        addPacketToSend(packet);
    }
} // namespace Nitwork
