/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Json
*/

#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "Logger.hpp"

extern "C"
{
#include "MessageTypes.h"
}

enum class JsonType {
    DEFAULT_PLAYER,
    DEFAULT_PARALLAX,
    WAVE,
    BULLETS,
    MENU,
    SELECT_LOBBY,
    CREATE_LOBBY,
    HUD,
    ENEMIES
};

const std::unordered_map<JsonType, std::string> pathToJson = {
    {JsonType::DEFAULT_PLAYER,   "assets/Json/playerData.json"  },
    {JsonType::DEFAULT_PARALLAX, "assets/Json/parallaxData.json"},
    {JsonType::MENU,             "assets/Json/menu.json"        },
    {JsonType::SELECT_LOBBY,     "assets/Json/selectLobby.json" },
    {JsonType::CREATE_LOBBY,     "assets/Json/createLobby.json" },
    {JsonType::HUD,              "assets/Json/HUD.json"         },
    {JsonType::WAVE,             "assets/Json/waves.json"        },
    {JsonType::BULLETS,          "assets/Json/bullets.json"     },
    {JsonType::ENEMIES,          "assets/Json/enemies.json"     },
};

class Json {
    public:
        static Json &getInstance();

        nlohmann::json getDataByJsonType(JsonType dataType);

        nlohmann::json getDataByJsonType(const std::string &index, JsonType dataType);

        nlohmann::json getDataByVector(const std::vector<std::string> &indexes, JsonType dataType);

        std::vector<nlohmann::json>
        getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);

        static bool isDataExist(nlohmann::json jsonData, const std::string &index);

        std::vector<nlohmann::json>
        getDatasFromList(const std::vector<nlohmann::json> &list, const std::string &key);

        std::vector<nlohmann::json> getDatasFromList(const nlohmann::json &list, const std::string &key);

        std::vector<nlohmann::json> getDatasFromList(const nlohmann::json &list);

        template <typename T>
        T getDataFromJson(nlohmann::json jsonData, const std::string &index)
        {
            if (jsonData[index] == nullptr) {
                Logger::fatal(std::string("(getDataByJson<template>) Key : " + index + " is not valid"));
                throw std::runtime_error("Json error");
            }
            return jsonData[index].get<T>();
        }

        template <typename T>
        std::vector<T> getObjectsIdInArray(JsonType type, const std::string &arrayName)
        {
            std::vector<T> ids;
            auto data = getDataByJsonType(type)[arrayName];

            for (const auto &object : data) {
                if (isDataExist(object, "id")) {
                    ids.push_back(object["id"].get<T>());
                }
            }
            return ids;
        }

        template <typename T>
        nlohmann::json getJsonObjectById(JsonType type, const T &id, const std::string &arrayName)
        {
            auto objectList = getDataByJsonType(type)[arrayName];

            for (const auto &object : objectList) {
                auto idField = object.find("id");
                if (idField != object.end() && idField->get<T>() == id) {
                    return object;
                }
            }
            Logger::fatal(std::string("(getJsonObject) Key : is not valid for array : " + arrayName));
            throw std::runtime_error("Json error");
        }

        template <typename T>
        T getDataByJsonType(const std::string &index, JsonType dataType)
        {
            return getDataFromJson<T>(_jsonDatas[dataType], index);
        }

        template <typename T>
        T getDataByVector(const std::vector<std::string> &indexes, JsonType dataType)
        {
            auto datas = getDataByJsonType(dataType);
            auto begin = indexes.begin();

            if (indexes.empty()) {
                Logger::fatal(std::string("(getDataByVector<T>): empty list"));
                throw std::runtime_error("Json error");
            }
            for (; begin + 1 != indexes.end(); begin++) {
                if (datas[*begin] == nullptr) {
                    Logger::fatal(std::string("(getDataByVector<T>) Key : " + *begin + " is not valid"));
                    throw std::runtime_error("Json error");
                }
                datas = datas[*begin];
            }
            return getDataFromJson<T>(datas, *(indexes.end() - 1));
        }

    private:
        Json()  = default;
        ~Json() = default;

        std::vector<nlohmann::json> &getDatasFromList(
            std::vector<nlohmann::json> &datas,
            nlohmann::json &listData,
            const std::string &key);

        bool isListData(const std::vector<nlohmann::json> &datas, const std::string &key);

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Json _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        static nlohmann::json loadJsonData(const std::string &path);

        std::unordered_map<JsonType, nlohmann::json> _jsonDatas;
};
