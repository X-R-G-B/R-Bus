/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ButtonCallbacks
*/

#include "Logger.hpp"
#include "ButtonCallbacks.hpp"
#include "NitworkClient.hpp"
#include "CustomTypes.hpp"
#include "SceneManager.hpp"


namespace Menu {
    namespace Callback {

        constexpr int PORT_MIN = 0;
        constexpr int PORT_MAX = 65535;

        static bool isNumber(const std::string &str)
        {
            return std::all_of(str.begin(), str.end(), ::isdigit);
        }

        static bool checkArgs(const std::string &ip, const std::string &port)
        {
            if (ip.empty()) {
                Logger::error("Invalid ip");
                return false;
            }
            if (port.empty()) {
                Logger::error("Invalid port");
                return (false);
            }
            if (!isNumber(port) || std::stoi(port) < PORT_MIN || std::stoi(port) > PORT_MAX) {
                Logger::error("Invalid port");
                return false;
            }
            return true;
        }

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
            std::string ip("");
            std::string port("");

            getIpAndPort(ip, port);
            if (!checkArgs(ip, port)) {
                return;
            }
            if (!Nitwork::NitworkClient::getInstance()
                     .startClient(std::stoi(port.c_str()), ip.c_str(), DEFAULT_THREAD_NB, TICKS)) {
                Logger::error("Error network couldn't connect");
                return;
            }
            Nitwork::NitworkClient::getInstance().addInitMsg();
            Nitwork::NitworkClient::getInstance().addReadyMsg();
            Scene::SceneManager::getInstance().changeScene(Scene::Scene::MAIN_GAME);
        }
    }
}