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
#include "Raylib.hpp"
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

        static bool isNumber(const std::string &str)
        {
            return std::all_of(str.begin(), str.end(), ::isdigit);
        }

        static bool getNameAndMaxNb(std::string &name, std::string &maxNbPlayer)
        {
            Registry::components<Types::InputBox> arrInputBox =
                Registry::getInstance().getComponents<Types::InputBox>();
            std::vector<std::size_t> ids =
                Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].name == "name" && name.empty()) {
                    name = arrInputBox[id].text;
                }
                if (arrInputBox[id].name == "maxNb" && maxNbPlayer.empty()) {
                    if (!isNumber(arrInputBox[id].text)) {
                        Logger::error("Max nb player is not a number (" + arrInputBox[id].text + ")");
                        return false;
                    }
                    if (std::stoi(arrInputBox[id].text) > 0) {
                        maxNbPlayer = arrInputBox[id].text;
                    }
                }
            }
            return !name.empty() && !maxNbPlayer.empty();
        }

        void onButtonCreateLobbyNormalClicked()
        {
            std::string name;
            std::string maxNvPlayer;

            if (getNameAndMaxNb(name, maxNvPlayer)) {
                unsigned int nbPlayer = static_cast<unsigned int>(std::stoi(maxNvPlayer));
                Nitwork::NitworkClient::getInstance().addCreateLobbyMsg(name, CLASSIC_GAME, nbPlayer);
            }
        }

        void onButtonGotoCreateLobbyClicked()
        {
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::CREATE_LOBBY);
        }

        void gotToSelectLobby()
        {
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::SELECT_LOBBY);
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
            if (!Nitwork::NitworkClient::getInstance().connectMainServer(ip, std::stoi(port))) {
                Logger::error("Cannot connect to server : Bad Ip or Port.");
            }
        }

        void connectLobbySelected()
        {
            auto arrInputBox = Registry::getInstance().getComponents<Types::InputBox>();
            auto arrLobbyStatus =
                Registry::getInstance().getComponents<::Systems::SelectLobbySystems::LobbyStatus>();
            auto ids = Registry::getInstance().getEntitiesByComponents(
                {typeid(Types::InputBox), typeid(::Systems::SelectLobbySystems::LobbyStatus)});

            for (auto id : ids) {
                if (arrInputBox[id].selected) {
                    if (arrLobbyStatus[id].port > 0 && arrLobbyStatus[id].ip != "") {
                        Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
                        Nitwork::NitworkClient::getInstance().connectLobby(
                            arrLobbyStatus[id].ip,
                            arrLobbyStatus[id].port);
                    }
                }
            }
        }

        static void resetSelectedLobby()
        {
            auto arrLobbyStatus =
                Registry::getInstance().getComponents<::Systems::SelectLobbySystems::LobbyStatus>();
            auto arrText = Registry::getInstance().getComponents<Raylib::Text>();
            auto ids     = Registry::getInstance().getEntitiesByComponents(
                {typeid(Raylib::Text), typeid(::Systems::SelectLobbySystems::LobbyStatus)});

            for (auto &id : ids) {
                arrLobbyStatus[id].ip   = "";
                arrLobbyStatus[id].port = -1;
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
            if (::Systems::SelectLobbySystems::LobbyStatus::pageNbr
                >= ::Systems::SelectLobbySystems::LobbyStatus::pageMax) {
                return;
            }
            resetSelectedLobby();
            ::Systems::SelectLobbySystems::LobbyStatus::pageNbr += 1;
        }

        void sendReadyPacket()
        {
            auto idsButton = Registry::getInstance().getEntitiesByComponents({typeid(Types::Button)});
            auto idsText = Registry::getInstance().getEntitiesByComponents({typeid(Raylib::Text)});
            auto arrText = Registry::getInstance().getComponents<Raylib::Text>();
            
            Nitwork::NitworkClient::getInstance().addReadyMsg();
            for (auto &id : idsButton) {
                Registry::getInstance().removeEntity(id);
            }
            for (auto &id : idsText) {
                Logger::info("Je remove : " + std::to_string(id));
                Registry::getInstance().removeEntity(id);
            }
        }
    } // namespace Callback
} // namespace Menu
