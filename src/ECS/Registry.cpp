/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"
#include <string>

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
Registry Registry::_instance = Registry();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

Registry &Registry::getInstance()
{
    return _instance;
}

std::size_t Registry::addEntity()
{
    for (auto function : _addComponentPlaceFunctions) {
        function(*this);
    }
    _entitiesNb++;
    return _entitiesNb - 1;
}

void Registry::removeEntity(std::size_t id)
{
    for (auto function : _removeComponentFunctions) {
        function(*this, id);
    }
}

void Registry::clear()
{
    _data.clear();
    _addComponentPlaceFunctions.clear();
    _removeComponentFunctions.clear();
    _getExistingsId.clear();
    _entitiesNb = 0;
}

static std::vector<std::size_t>
match(std::vector<std::size_t> fst, std::vector<std::size_t> scd)
{
    std::vector<std::size_t> res;

    for (auto it = fst.begin(); it != fst.end(); it++) {
        for (auto scdIt = scd.begin(); scdIt != scd.end(); scdIt++) {
            if (*it == *scdIt) {
                res.push_back(*it);
            }
        }
    }
    return res;
}

std::vector<std::size_t> Registry::getExistings(std::type_index type)
{
    auto funcIt = _getExistingsId.find(type);
    if (funcIt == _getExistingsId.end()) {
        return {};
    }
    return (funcIt->second)(*this);
}

std::vector<std::size_t>
Registry::getEntitiesByComponents(std::vector<std::type_index> types)
{
    auto it                      = types.begin();
    std::vector<std::size_t> res = getExistings(*it);
    it++;

    for (; it != types.end(); it++) {
        res = match(res, getExistings(*it));
    }
    return res;
}

void Registry::setToBackLayers(std::size_t id, BackLayers layer)
{
    removeFromDefaultLayer(id);
    _backLayers[layer].push_back(id);
}

void Registry::setToDefaultLayer(std::size_t id)
{
    _defaultLayer.push_back(id);
}

void Registry::setToFrontLayers(std::size_t id, FrontLayers layer)
{
    _backLayers[layer].push_back(id);
}

std::vector<std::vector<std::size_t>> Registry::getBackLayers()
{
    return _backLayers;
}

std::vector<std::size_t> Registry::getDefaultLayer()
{
    return _defaultLayer;
}

std::vector<std::vector<std::size_t>> Registry::getFrontLayers()
{
    return _frontLayers;
}

void Registry::initLayers(bool back)
{
    std::size_t max = 0;
    if (back) {
        max = static_cast<std::size_t>(BackLayers::BACKMAX);
    } else {
        max = static_cast<std::size_t>(FrontLayers::FRONTMAX);
    }

    for (std::size_t i = 0; i < max; i++) {
        std::vector<std::vector<std::size_t>> &layers =
            back ? _backLayers : _frontLayers;
        layers.emplace_back();
    }
    if (back) {
        initLayers(false);
    }
}

void Registry::removeFromDefaultLayer(std::size_t id)
{
    auto i = _defaultLayer.end();

    while (i != _defaultLayer.begin()) {
        --i;
        if (*i == id) {
            _defaultLayer.erase(i);
            break;
        }
    }
}

Registry::Registry() : _entitiesNb(0)
{
    initLayers(true);
}
