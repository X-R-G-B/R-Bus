/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Json
*/

#include "Json.hpp"
#include <fstream>
#include <sstream>
#include <utility>
#include "Logger.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
Json Json::_instance = Json();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

nlohmann::json Json::loadJsonData(const std::string &path)
{
    std::ifstream fileData(path);
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

nlohmann::basic_json<> Json::getDataByVector(const std::vector<std::string> &indexes, JsonType dataType)
{
    nlohmann::basic_json<> finalData(_jsonDatas[dataType]);

    for (auto &key : indexes) {
        finalData = finalData[key];
        if (finalData == nullptr) {
            Logger::error(std::string("Key : " + key + " is not valid"));
        }
        if (finalData.is_array() == true) {
            return (finalData);
        }
    }

    return (finalData);
}

nlohmann::basic_json<> Json::getDataByJsonType(JsonType dataType)
{
    nlohmann::basic_json<> data(_jsonDatas[dataType]);

    return (data);
}

nlohmann::basic_json<> &Json::getDataFromJson(nlohmann::basic_json<> jsonData, const std::string &index)
{
    if (jsonData[index] == nullptr) {
        Logger::error(std::string("Key : " + index + " is not valid"));
    }
    return jsonData[index];
}

nlohmann::basic_json<> Json::getDataByJsonType(const std::string &index, JsonType dataType)
{
    nlohmann::basic_json<> finalData(_jsonDatas[dataType]);

    finalData = finalData[index];
    if (finalData == nullptr) {
        Logger::error(std::string("Key : " + index + " is not valid"));
    }
    return (finalData);
}

std::vector<nlohmann::basic_json<>>
Json::getDatasFromList(const std::vector<nlohmann::basic_json<>> &list, const std::string &key)
{
    std::vector<nlohmann::basic_json<>> datas;
    std::vector<nlohmann::basic_json<>> tmp;

    for (auto &elem : list) {
        if (elem[key].is_array() == true) {
            tmp = getDatasFromList(elem[key]);
            datas.insert(datas.end(), tmp.begin(), tmp.end());
        } else {
            datas.push_back(elem[key]);
        }
    }
    return (datas);
}

std::vector<nlohmann::basic_json<>>
Json::getDatasFromList(const nlohmann::basic_json<> &list, const std::string &key)
{
    std::vector<nlohmann::basic_json<>> datas;

    for (auto &elem : list) {
        datas.push_back(elem[key]);
    }
    return (datas);
}

std::vector<nlohmann::basic_json<>> Json::getDatasFromList(const nlohmann::basic_json<> &list)
{
    std::vector<nlohmann::basic_json<>> datas;

    for (auto &elem : list) {
        datas.push_back(elem);
    }
    return (datas);
}

std::vector<nlohmann::basic_json<>> &Json::getDatasFromList(
    std::vector<nlohmann::basic_json<>> &datas,
    nlohmann::basic_json<> &listData,
    const std::string &key)
{
    if (datas.empty()) {
        datas = getDatasFromList(listData);
        datas = getDatasFromList(datas, key);
    } else {
        datas = getDatasFromList(datas, key);
    }
    return (datas);
}

std::vector<nlohmann::basic_json<>>
Json::getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType)
{
    nlohmann::basic_json<> &finalData(_jsonDatas[dataType]);
    std::vector<nlohmann::basic_json<>> datas;

    for (auto &key : indexes) {
        if (finalData.is_array() == true || datas.empty() == false) {
            datas = getDatasFromList(datas, finalData, key);
        } else {
            finalData = finalData[key];
        }
    }
    if (datas.empty()) {
        datas.push_back(finalData);
    }
    return datas;
}

Json &Json::getInstance()
{
    return _instance;
}

Json::Json()
{
    for (auto &it : pathToJson) {
        _jsonDatas.insert({it.first, loadJsonData(it.second)});
    }
}

Json::~Json()
{
}
