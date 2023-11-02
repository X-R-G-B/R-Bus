/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ButtonCallbacks
*/

#pragma once

#include <functional>
#include <unordered_map>
#include "Json.hpp"

namespace Menu {
    namespace Callback {

        enum CallbackType {
            DEFAULT_CALLBACK,
            INIT_CONNECTION,
            CONNECT,
            CREATE,
            GO_BACK,
            GO_NEXT,
            GO_SELECT_LOBBY,
            CREATE_NORMAL,
            SEND_READY
        };

        NLOHMANN_JSON_SERIALIZE_ENUM(
            CallbackType,
            {
                {DEFAULT_CALLBACK, nullptr                           },
                {INIT_CONNECTION,  "initConnection"                  },
                {CONNECT,          "connectLobbySelected"            },
                {CREATE,           "onButtonGoToCreateLobbyClicked"  },
                {GO_BACK,          "goBackPage"                      },
                {GO_NEXT,          "goNextPage"                      },
                {GO_SELECT_LOBBY,  "goToSelectLobby"                 },
                {CREATE_NORMAL,    "onButtonCreateLobbyNormalClicked"},
                {SEND_READY,    "sendReadyPacket"}
        });

        void initConnection();

        void defaultCallBack();

        void connectLobbySelected();

        void goBackPage();

        void goNextPage();

        void onButtonGotoCreateLobbyClicked();

        void gotToSelectLobby();

        void onButtonCreateLobbyNormalClicked();

        void sendReadyPacket();

        const std::unordered_map<CallbackType, std::function<void()>> callbacks = {
            {CallbackType::DEFAULT_CALLBACK, &defaultCallBack                 },
            {CallbackType::INIT_CONNECTION,  &initConnection                  },
            {CallbackType::CONNECT,          &connectLobbySelected            },
            {CallbackType::CREATE,           &onButtonGotoCreateLobbyClicked  },
            {CallbackType::GO_SELECT_LOBBY,  &gotToSelectLobby                },
            {CallbackType::GO_BACK,          &goBackPage                      },
            {CallbackType::GO_NEXT,          &goNextPage                      },
            {CallbackType::CREATE_NORMAL,    &onButtonCreateLobbyNormalClicked},
            {CallbackType::SEND_READY,    &sendReadyPacket}
        };

    } // namespace Callback
} // namespace Menu
