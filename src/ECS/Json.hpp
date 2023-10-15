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

enum class JsonType {
    DEFAULT_ENEMY,
    DEFAULT_PLAYER,
    DEFAULT_PARALLAX,
};

const std::unordered_map<JsonType, std::string> pathToJson = {
    {JsonType::DEFAULT_ENEMY,    "assets/Json/enemyData.json"   },
    {JsonType::DEFAULT_PLAYER,   "assets/Json/playerData.json"  },
    {JsonType::DEFAULT_PARALLAX, "assets/Json/parallaxData.json"}
};

class Json {
    public:
        static Json &getInstance();

        nlohmann::basic_json<> getDataByJsonType(JsonType dataType);

        nlohmann::basic_json<> getDataByJsonType(const std::string &index, JsonType dataType);

        nlohmann::basic_json<> getDataByVector(const std::vector<std::string> &indexes, JsonType dataType);

        std::vector<nlohmann::basic_json<>>
        getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);

        static bool isDataExist(nlohmann::basic_json<> jsonData, const std::string &index);

        std::vector<nlohmann::basic_json<>>
        getDatasFromList(const std::vector<nlohmann::basic_json<>> &list, const std::string &key);

        std::vector<nlohmann::basic_json<>>
        getDatasFromList(const nlohmann::basic_json<> &list, const std::string &key);

        std::vector<nlohmann::basic_json<>> getDatasFromList(const nlohmann::basic_json<> &list);

        template <typename T>
        T getDataFromJson(nlohmann::basic_json<> jsonData, const std::string &index)
        {
            if (jsonData[index] == nullptr) {
                Logger::error(std::string("Key : " + index + " is not valid"));
            }
            return jsonData[index].get<T>();
        }

    private:
        Json() = default;
        ~Json() = default;

        std::vector<nlohmann::basic_json<>> &getDatasFromList(
            std::vector<nlohmann::basic_json<>> &datas,
            nlohmann::basic_json<> &listData,
            const std::string &key);

        bool isListData(const std::vector<nlohmann::basic_json<>> &datas, const std::string &key);

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Json _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        static nlohmann::json loadJsonData(const std::string &path);

        std::unordered_map<JsonType, nlohmann::json> _jsonDatas;
};
