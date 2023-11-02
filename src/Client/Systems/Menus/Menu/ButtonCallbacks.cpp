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
#include "Raylib.hpp"
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

            for (auto id : ids) {
                if (arrInputBox[id].selected) {
                    if (arrLobbyStatus[id].port > 0 && arrLobbyStatus[id].ip != "") {
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
                        Nitwork::NitworkClient::getInstance().connectLobby(arrLobbyStatus[id].ip, arrLobbyStatus[id].port);
                    }
                }
            }
        }

        static void resetSelectedLobby()
        {
            auto arrLobbyStatus = Registry::getInstance().getComponents<::Systems::SelectLobbySystems::LobbyStatus>();
            auto arrText = Registry::getInstance().getComponents<Raylib::Text>();
            auto ids = Registry::getInstance().getEntitiesByComponents({typeid(Raylib::Text), typeid(::Systems::SelectLobbySystems::LobbyStatus)});

            for (auto &id : ids) {
                arrLobbyStatus[id].ip = "";
                arrLobbyStatus[id].port = -1;
                Logger::info("RESETING ...");
                arrText[id].setCurrentText(std::string(""));
            }
        }

        void goBackPage()
        {
            if (::Systems::SelectLobbySystems::LobbyStatus::pageNbr <= 1) {
                return;
            }
            resetSelectedLobby();
            ::Systems::SelectLobbySystems::LobbyStatus::pageNbr -= 1;
        }

        void goNextPage()
        {
            if (::Systems::SelectLobbySystems::LobbyStatus::pageNbr >= ::Systems::SelectLobbySystems::LobbyStatus::pageMax) {
                return;
            }
            resetSelectedLobby();
            ::Systems::SelectLobbySystems::LobbyStatus::pageNbr += 1;
        }
    } // namespace Callback
} // namespace Menu
