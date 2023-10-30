/*
** EPITECH PROJECT, 2023
** r-type
** File description:
** main_server_args_handling
*/

#pragma once
#include <vector>
extern "C"
{
#include "MessageTypes.h"
}
#include "Logger.hpp"

namespace Args {
    constexpr int PORT_MIN   = 0;
    constexpr int PORT_MAX   = 65535;
    constexpr int SHOW_HELP  = -2;
    constexpr int WRONG_ARGS = -1;

    constexpr int MAIN_SERVER                 = 0;
    static constexpr auto MAIN_SERVER_ARG     = "0";
    static constexpr int MAX_MAIN_SERVER_ARGS = 2;

    constexpr int LOBBY_SERVER               = 1;
    static constexpr auto LOBBY_SERVER_ARG   = "1";
    static constexpr int MAX_LOBBY_ARGS      = 6;
    static constexpr int MAX_LOBBY_NAME_SIZE = 20;
    static constexpr int MAX_LOBBY_IP_SIZE   = 15;

    class ServerArgsHandling {
        public:
            static int checkArgs(int ac, const char **av)
            {
                if (ac == 1) {
                    return SHOW_HELP;
                }
                const std::vector<std::string> args(av + 1, av + ac);
                if (args.front() != MAIN_SERVER_ARG && args.front() != LOBBY_SERVER_ARG) {
                    Logger::error("Invalid server type: " + args[0]);
                    return WRONG_ARGS;
                }
                if (args.front() == MAIN_SERVER_ARG) {
                    return checkMainServerArgs(args);
                } else if (args.front() == LOBBY_SERVER_ARG) {
                    return checkLobbyServerArgs(args);
                }
                return WRONG_ARGS;
            }

            static void help()
            {
                Logger::info("Usage: ./r-type_server <serverType> <serverArgs>");
                Logger::info("serverType: 0 for main server, 1 for lobby server");
                Logger::info("serverArg:");
                Logger::info("\t- 0 <port> for main server");
                Logger::info("\t- 1 <playerNb> <gameType> <serverName> <ownerIp> <ownerPort> for lobby server");
                Logger::info("Example (main server): ./r-type_server 0 4242");
                Logger::info("Example (lobby server): ./r-type_server 1 4 0 serverDeXavier 172.20.10.5 4242");
            }

        private:
            static bool isNumber(const std::string &str)
            {
                return std::all_of(str.begin(), str.end(), ::isdigit);
            }

            static int checkMainServerArgs(const std::vector<std::string> &args)
            {
                if (args.size() != MAX_MAIN_SERVER_ARGS) {
                    Logger::error("Invalid number of arguments");
                    return WRONG_ARGS;
                }
                if (!isNumber(args[1]) || std::stoi(args[1]) < PORT_MIN || std::stoi(args[1]) > PORT_MAX) {
                    Logger::error("Invalid port: " + args[1]);
                    return WRONG_ARGS;
                }
                return MAIN_SERVER;
            }

            static int checkLobbyServerArgs(const std::vector<std::string> &args)
            {
                if (args.size() != MAX_LOBBY_ARGS) {
                    Logger::error("Invalid number of arguments");
                    return WRONG_ARGS;
                }
                if (!isNumber(args[1]) || std::stoi(args[1]) < 1) {
                    Logger::error("Invalid playerNb: " + args[1]);
                    return WRONG_ARGS;
                }
                if (!isNumber(args[2]) || std::stoi(args[2]) < 0 || std::stoi(args[2]) >= MAX_GAME_TYPE) {
                    Logger::error("Invalid game type: " + args[2]);
                    return WRONG_ARGS;
                }
                if (args[3].size() > MAX_LOBBY_NAME_SIZE) {
                    Logger::error("Invalid serverName: " + args[2]);
                    return WRONG_ARGS;
                }
                if (args[4].size() > MAX_LOBBY_IP_SIZE) {
                    Logger::error("Invalid ownerIp: " + args[3]);
                    return WRONG_ARGS;
                }
                if (!isNumber(args[5]) || std::stoi(args[5]) < PORT_MIN || std::stoi(args[5]) > PORT_MAX) {
                    Logger::error("Invalid ownerPort: " + args[5]);
                    return WRONG_ARGS;
                }
                return LOBBY_SERVER;
            }
    };
} // namespace Args
