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
#include "ResourcesManager.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
Json Json::_instance = Json();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

nlohmann::json Json::loadJsonData(const std::string &path)
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

nlohmann::json Json::getDataByVector(const std::vector<std::string> &indexes, JsonType dataType)
{
    nlohmann::json finalData(_jsonDatas[dataType]);

    for (const auto &key : indexes) {
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

nlohmann::json Json::getDataByJsonType(JsonType dataType)
{
    nlohmann::json data(_jsonDatas[dataType]);

    return (data);
}

nlohmann::basic_json<> Json::getDataByJsonType(const std::string &index, JsonType dataType)
{
    nlohmann::basic_json<> finalData(_jsonDatas[dataType]);

    finalData = finalData[index];
    if (finalData == nullptr) {
        Logger::error(std::string("(getDataByJsonType) Key : " + index + " is not valid"));
    }
    return (finalData);
}

std::vector<nlohmann::json>
Json::getDatasFromList(const std::vector<nlohmann::json> &list, const std::string &index)
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

std::vector<nlohmann::json> Json::getDatasFromList(const nlohmann::json &list, const std::string &index)
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

nlohmann::json &Json::getDataFromJson(nlohmann::json jsonData, const std::string &index)
{
    if (jsonData[index] == nullptr) {
        throw std::runtime_error("Json error");
    }
    return (jsonData[index]);
}

std::vector<nlohmann::json> Json::getDatasFromList(const nlohmann::json &list)
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

std::vector<nlohmann::json> &
Json::getDatasFromList(std::vector<nlohmann::json> &datas, nlohmann::json &listData, const std::string &key)
{
    if (datas.empty()) {
        datas = getDatasFromList(listData);
        datas = getDatasFromList(datas, key);
    } else {
        datas = getDatasFromList(datas, key);
    }
    return (datas);
}

bool Json::isDataExist(nlohmann::basic_json<> jsonData, const std::string &index)
{
    if (jsonData[index] == nullptr) {
        return (false);
    }
    return (true);
}

std::vector<nlohmann::json>
Json::getDatasByJsonType(const std::vector<std::string> &indexes, JsonType dataType)
{
    nlohmann::json &finalData(_jsonDatas[dataType]);
    std::vector<nlohmann::json> datas;

    for (const auto &key : indexes) {
        if (finalData.is_array() || !datas.empty()) {
            datas = getDatasFromList(datas, finalData, key);
            continue;
        }
        if (finalData[key] == nullptr) {
            Logger::fatal(std::string("(getDatasByJsonType) Key : " + key + " is not valid"));
            throw std::runtime_error("Json error");
        }
        finalData = finalData[key];
    }
    if (datas.empty()) {
        datas.push_back(finalData);
    }
    return datas;
}

Json &Json::getInstance()
{
    if (_instance._jsonDatas.empty()) {
        for (const auto &it : pathToJson) {
            _instance._jsonDatas.insert({it.first, _instance.loadJsonData(it.second)});
        }
    }
    return _instance;
}
