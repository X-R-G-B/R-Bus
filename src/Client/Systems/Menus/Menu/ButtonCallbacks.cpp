/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ButtonCallbacks
*/

#include "ButtonCallbacks.hpp"
#include "CustomTypes.hpp"
#include "Logger.hpp"
#include "NitworkClient.hpp"
#include "SceneManager.hpp"
#include "SelectLobbySystems.hpp"

namespace Menu {
    namespace Callback {

        static void getIpAndPort(std::string &ip, std::string &port)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});
            for (auto id : ids) {
                if (arrInputBox[id].name == "ip") {
                    ip = arrInputBox[id].text;
                }
                if (arrInputBox[id].name == "port") {
                    port = arrInputBox[id].text;
                }
            }
        }

        void defaultCallBack()
        {
            Logger::debug("Clicked");
        }

        void initConnection()
        {
            std::string ip;
            std::string port;

            getIpAndPort(ip, port);
            Nitwork::NitworkClient::getInstance().connectMainServer(ip, std::stoi(port));
        }

        void connectLobbySelected()
        {
            auto arrInputBox = Registry::getInstance().getComponents<Types::InputBox>();
            auto arrLobbyStatus = Registry::getInstance().getComponents<::Systems::SelectLobbySystems::LobbyStatus>();
            auto ids = Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox), typeid(::Systems::SelectLobbySystems::LobbyStatus)});

            Logger::info("Calling callback");
            for (auto id : ids) {
                Logger::info(std::string(std::to_string(arrInputBox[id].selected)));
                if (arrInputBox[id].selected) {
                    Logger::info("Someone is selected !");
                    if (arrLobbyStatus[id].port > 0 && arrLobbyStatus[id].ip != "") {
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
                        Logger::info("IP TO CONNECT : " + arrLobbyStatus[id].ip);
                        Logger::info("IP TO CONNECT : " + std::to_string(arrLobbyStatus[id].port));
                        Nitwork::NitworkClient::getInstance().connectLobby(arrLobbyStatus[id].ip, arrLobbyStatus[id].port);
                    }
                }
            }
        }
    } // namespace Callback
} // namespace Menu
