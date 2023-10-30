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
#include <fstream>
#include <iostream>
#include <sstream>
#include <utility>
#include "Logger.hpp"
#include "ResourcesManager.hpp"

extern "C"
{
#include "MessageTypes.h"
}

enum class JsonType { DEFAULT_ENEMY, DEFAULT_PLAYER, DEFAULT_PARALLAX, TERMINATOR, WAVE, BULLETS };

const std::unordered_map<enemy_type_e, JsonType> messageTypes = {
    {CLASSIC_ENEMY, JsonType::DEFAULT_ENEMY},
    {TERMINATOR,    JsonType::TERMINATOR   }
};

const std::unordered_map<JsonType, std::string> pathToJson = {
    {JsonType::DEFAULT_ENEMY,    "assets/Json/enemyData.json"   },
    {JsonType::DEFAULT_PLAYER,   "assets/Json/playerData.json"  },
    {JsonType::DEFAULT_PARALLAX, "assets/Json/parallaxData.json"},
    {JsonType::TERMINATOR,       "assets/Json/terminator.json"  },
    {JsonType::WAVE,             "assets/Json/wave.json"        },
    {JsonType::BULLETS,          "assets/Json/bullets.json"     }
};

class Json {
    public:
        static Json &getInstance()
        {
            static auto instance = Json();
            if (instance._jsonDatas.empty()) {
                for (const auto &it : pathToJson) {
                    instance._jsonDatas.insert({it.first, instance.loadJsonData(it.second)});
                }
            }
            return instance;
        }

        nlohmann::json
        getJsonObjectById(JsonType type, const std::string &id, const std::string &arrayName)
        {
            auto objectList = getDataByJsonType(type)[arrayName];

            for (const auto &object : objectList) {
                auto idField = object.find("id");
                if (idField != object.end() && idField->is_string() && *idField == id) {
                    return object;
                }
            }
            Logger::fatal(std::string("(getJsonObject) Key : " + id + " is not valid"));
            throw std::runtime_error("Json error");
        }

        template <typename T>
        T getDataByJsonType(const std::string &index, JsonType dataType)
        {
            nlohmann::json finalData(_jsonDatas[dataType]);

            for (const auto &key : index) {
                finalData = finalData[key];
                if (finalData == nullptr) {
                    Logger::fatal(std::string("(getDataByVector) Key : " + key + " is not valid"));
                    throw std::runtime_error("Json error");
                }
                if (finalData.is_array()) {
                    return (finalData);
                }
            }

            return (finalData);
        }

        nlohmann::json getDataByJsonType(JsonType dataType)
        {
            nlohmann::json data(_jsonDatas[dataType]);

            return (data);
        }

        nlohmann::basic_json<> getDataByJsonType(const std::string &index, JsonType dataType)
        {
            nlohmann::basic_json<> finalData(_jsonDatas[dataType]);

            finalData = finalData[index];
            if (finalData == nullptr) {
                Logger::error(std::string("(getDataByJsonType) Key : " + index + " is not valid"));
            }
            return (finalData);
        }

        std::vector<nlohmann::json>
        getDatasFromList(const std::vector<nlohmann::json> &list, const std::string &index)
        {
            std::vector<nlohmann::json> datas;
            std::vector<nlohmann::json> tmp;

            for (const auto &elem : list) {
                if (elem[index] == nullptr) {
                    Logger::fatal(std::string("(getDatasDromList : 2) Key : " + index + " is not valid"));
                    throw std::runtime_error("Json error");
                }
                if (elem[index].is_array() == true) {
                    tmp = getDatasFromList(elem[index]);
                    datas.insert(datas.end(), tmp.begin(), tmp.end());
                } else {
                    datas.push_back(elem[index]);
                }
            }
            return (datas);
        }

        std::vector<nlohmann::json> getDatasFromList(const nlohmann::json &list, const std::string &index)
        {
            std::vector<nlohmann::json> datas;

            for (const auto &elem : list) {
                if (elem[index] == nullptr) {
                    Logger::fatal(std::string("(getDatasFromList : 1) Key : " + index + " is not valid"));
                    throw std::runtime_error("Json error");
                }
                datas.push_back(elem[index]);
            }
            return (datas);
        }

        std::vector<nlohmann::json> getDatasFromList(const nlohmann::json &list)
        {
            std::vector<nlohmann::json> datas;

            if (list.is_array() == false) {
                Logger::fatal(std::string("(getDatasFromList : 3) Conversion to list is not possible"));
                throw std::runtime_error("Json error");
            }
            for (const auto &elem : list) {
                datas.push_back(elem);
            }
            return (datas);
        }

    bool isDataExist(nlohmann::basic_json<> jsonData, const std::string &index)
    {
        if (jsonData[index] == nullptr) {
            return (false);
        }
        return (true);
    }

    std::vector<nlohmann::json>
    getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType)
    {
        nlohmann::json finalData = getDataByJsonType(dataType);
        std::vector<nlohmann::json> datas;

        for (const auto &key : indexes) {
            if (finalData.is_array() || !datas.empty()) {
                datas = getDatasFromList(datas, finalData, key);
                continue;
            }
            if (finalData[key] == nullptr) {
                throw std::runtime_error("Json error");
            }
            finalData = finalData[key];
        }
        if (datas.empty()) {
            datas.push_back(finalData);
        }
        return datas;
    }


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

    nlohmann::json getDataByVector(const std::vector<std::string> &indexes, JsonType dataType)
    {
        return getDataByVector<nlohmann::json>(indexes, dataType);
    }

private:
    Json()  = default;
    ~Json() = default;

    std::vector<nlohmann::json> &
    getDatasFromList(std::vector<nlohmann::json> &datas, nlohmann::json &listData, const std::string &key)
    {
        if (datas.empty()) {
            datas = getDatasFromList(listData);
            datas = getDatasFromList(datas, key);
        } else {
            datas = getDatasFromList(datas, key);
        }
        return (datas);
    }

    static nlohmann::json loadJsonData(const std::string &path)
    {
        std::ifstream fileData(ECS::ResourcesManager::convertPath(path));
        std::ostringstream input;
        nlohmann::json jsonData = {};

        if (fileData.is_open()) {
            input << fileData.rdbuf();
            if (nlohmann::json::accept(input.str())) {
                jsonData = nlohmann::json::parse(input.str());
                return jsonData;
            }
        }
        return jsonData;
    }

    std::unordered_map<JsonType, nlohmann::json> _jsonDatas;
};
