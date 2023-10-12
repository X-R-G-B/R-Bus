/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Json
*/

#pragma once
#include <unordered_map>
#include <string>
#include <nlohmann/json.hpp>

enum class JsonType {
    DEFAULT_ENNEMY,
    DEFAULT_PLAYER,
    DEFAULT_PARALLAX,
};

const std::unordered_map<JsonType, std::string> pathToJson = {
    {JsonType::DEFAULT_ENNEMY, "assets/Json/ennemyData.json"},
    {JsonType::DEFAULT_PLAYER, "assets/Json/playerData.json"},
    {JsonType::DEFAULT_PARALLAX, "assets/Json/parallaxData.json"}
};

class Json {
    public:
        static Json &getInstance();
        ~Json();

        nlohmann::json_abi_v3_11_2::basic_json<> getDataByJsonType(const std::string &index, JsonType dataType);

        std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType);

        nlohmann::json_abi_v3_11_2::basic_json<> getDataByJsonType(const std::vector<std::string> &indexes, JsonType dataType);

        std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> getDatasFromList(const std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> &list, const std::string &key);

        std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> getDatasFromList(const nlohmann::json_abi_v3_11_2::basic_json<> &list, const std::string &key);

        std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> getDatasFromList(const nlohmann::json_abi_v3_11_2::basic_json<> &list);

    protected:
    private:
        Json();

        std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> &getDatasFromList(std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> &datas, nlohmann::json_abi_v3_11_2::basic_json<> &listData, const std::string &key);

        bool isListData(const std::vector<nlohmann::json_abi_v3_11_2::basic_json<>> &datas, const std::string &key);

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Json _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        nlohmann::json loadJsonData(const std::string &path);

        std::unordered_map<JsonType, nlohmann::json> _jsonDatas;
};
