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
            if (!Nitwork::NitworkClient::getInstance()
                     .start(std::stoi(port), DEFAULT_THREAD_NB, TICKS, ip)) {
                Logger::error("Error network couldn't connect");
                return;
            }
            Nitwork::NitworkClient::getInstance().addInitMsg();
            Nitwork::NitworkClient::getInstance().addReadyMsg();
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
        }
    } // namespace Callback
} // namespace Menu
