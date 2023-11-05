/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ButtonCallbacks
*/

#include "ButtonCallbacks.hpp"
#include "B-luga-graphics/GraphicsCustomTypes.hpp"
#include "B-luga-graphics/Raylib/Raylib.hpp"
#include "B-luga/Logger.hpp"
#include "B-luga/SceneManager.hpp"
#include "B-luga/SystemManagers/SystemManagersDirector.hpp"
#include "NitworkClient.hpp"
#include "Parallax.hpp"
#include "ResourcesManager.hpp"
#include "SelectLobbySystems.hpp"
#include "init.hpp"

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
            Scene::SceneManager::getInstance().changeScene(CREATE_LOBBY_SCENE);
        }

        void gotToSelectLobby()
        {
            Scene::SceneManager::getInstance().changeScene(SELECT_LOBBY);
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
            auto arrText = Registry::getInstance().getComponents<Raylib::TextShared>();
            auto ids     = Registry::getInstance().getEntitiesByComponents(
                {typeid(Raylib::TextShared), typeid(::Systems::SelectLobbySystems::LobbyStatus)});

            for (auto &id : ids) {
                arrLobbyStatus[id].ip   = "";
                arrLobbyStatus[id].port = -1;
                arrText[id]->setCurrentText(std::string(""));
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
            auto arrButton = Registry::getInstance().getComponents<Types::Button>();

            Nitwork::NitworkClient::getInstance().addReadyMsg();
            for (auto &id : idsButton) {
                Registry::getInstance().removeEntity(id);
            }
            auto &arrText = Registry::getInstance().getComponents<Raylib::TextShared>();
            auto idsText  = Registry::getInstance().getEntitiesByComponents({typeid(Raylib::TextShared)});
            for (auto &id : idsText) {
                if (arrText[id]->getCurrentText() == "READY !") {
                    Registry::getInstance().removeEntity(id);
                }
            }
        }

        void goCreateServer()
        {
            Scene::SceneManager::getInstance().changeScene(CREATE_SERVER_SCENE);
        }

        void goMenu()
        {
            Scene::SceneManager::getInstance().changeScene(MENU);
        }

        void createServer()
        {
            auto arrInputBox = Registry::getInstance().getComponents<Types::InputBox>();
            auto ids         = Registry::getInstance().getEntitiesByComponents({typeid(Types::InputBox)});

            for (auto id : ids) {
                if (arrInputBox[id].name == "port") {
                    Nitwork::NitworkClient::getInstance().createForkedServer(arrInputBox[id].text);
                }
            }
        }

        static const std::size_t MAX_PARALLAX = 2;

        void changeParallax()
        {

            auto ids = Registry::getInstance().getEntitiesByComponents({typeid(Raylib::TextShared)});
            auto idsPara = Registry::getInstance().getEntitiesByComponents({typeid(Types::Parallax)});
            auto arrText = Registry::getInstance().getComponents<Raylib::TextShared>();
            auto arrParallax = Registry::getInstance().getComponents<Types::Parallax>();

            if (Systems::Parallax::ActualParallax::getInstance()._actualParallaxNbr < MAX_PARALLAX) {
                Systems::Parallax::ActualParallax::getInstance()._actualParallaxNbr += 1;
            } else {
                Systems::Parallax::ActualParallax::getInstance()._actualParallaxNbr = 1;
            }
            for (auto id: idsPara) {
                Registry::getInstance().addToRemove(id);
            }
            switch(Systems::Parallax::ActualParallax::getInstance().getActualParallaxType()) {
                case JsonType::DEFAULT_PARALLAX: Systems::Parallax::ActualParallax::getInstance().setActualParralaxType(JsonType::PARALLAX_2); break;
                case JsonType::PARALLAX_2: Systems::Parallax::ActualParallax::getInstance().setActualParralaxType(JsonType::DEFAULT_PARALLAX); break;
                default: Systems::Parallax::ActualParallax::getInstance().setActualParralaxType(JsonType::DEFAULT_PARALLAX); break;
            }
            for (auto id : ids) {
                if (arrText[id]->getCurrentText().find("PARALLAX") != std::string::npos) {
                    Logger::info(
                        "parallax name"
                        + std::to_string(
                            Systems::Parallax::ActualParallax::getInstance()._actualParallaxNbr));
                    std::string paraName =
                        "PARALLAX_"
                        + std::to_string(
                            Systems::Parallax::ActualParallax::getInstance()._actualParallaxNbr);

                    arrText[id]->setCurrentText(paraName);
                }
            }
            Systems::Parallax::initParalax();
        }
    } // namespace Callback
} // namespace Menu
