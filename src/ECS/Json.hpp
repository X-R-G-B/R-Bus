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

        nlohmann::json getDataByJsonType(JsonType dataType);

        nlohmann::json getDataByJsonType(const std::string &index, JsonType dataType);

        nlohmann::json getDataByVector(const std::vector<std::string> &indexes, JsonType dataType);

        std::vector<nlohmann::json>
        getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);

        nlohmann::json &getDataFromJson(nlohmann::json jsonData, const std::string &index);

        bool isDataExist(nlohmann::json jsonData, const std::string &index);

        std::vector<nlohmann::json>
        getDatasFromList(const std::vector<nlohmann::json> &list, const std::string &key);

        std::vector<nlohmann::json>
        getDatasFromList(const nlohmann::json &list, const std::string &key);

        std::vector<nlohmann::json> getDatasFromList(const nlohmann::json &list);

        template <typename T>
        T getDataFromJson(nlohmann::json jsonData, const std::string &index)
        {
            if (jsonData[index] == nullptr) {
                Logger::fatal(std::string("Key : " + index + " is not valid"));
            }
            return jsonData[index].get<T>();
        }

    private:
        Json();
        ~Json() = default;

        std::vector<nlohmann::json> &getDatasFromList(
            std::vector<nlohmann::json> &datas,
            nlohmann::json &listData,
            const std::string &key);

        bool isListData(const std::vector<nlohmann::json> &datas, const std::string &key);

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Json _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        nlohmann::json loadJsonData(const std::string &path);

        std::unordered_map<JsonType, nlohmann::json> _jsonDatas;
};
