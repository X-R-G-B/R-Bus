/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"
#include <string>
#include "Clock.hpp"

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

static bool removeEntityFromLayer(std::size_t id, std::vector<std::size_t> &list)
{
    for (auto it = list.begin(); it != list.end();) {
        if (*it == id) {
            it = list.erase(it);
            continue;
        } else if (*it > id) {
            (*it)--;
        }
        it++;
    }
    return false;
}

void Registry::removeEntity(std::size_t id)
{
#ifdef CLIENT
    unloadRaylibComponents(id);
#endif
    for (auto function : _removeComponentFunctions) {
        function(*this, id);
    }
    for (auto &layer : _backLayers) {
        removeEntityFromLayer(id, layer);
    }
    removeEntityFromLayer(id, _defaultLayer);
    ;
    for (auto &layer : _frontLayers) {
        removeEntityFromLayer(id, layer);
    }
}

void Registry::clear()
{
#ifdef CLIENT
    for (std::size_t i = 0; i < _entitiesNb; i++) {
        unloadRaylibComponents(i);
    }
#endif
    _data.clear();
    _addComponentPlaceFunctions.clear();
    _removeComponentFunctions.clear();
    _getExistingsId.clear();
    for (auto &layer : _backLayers) {
        layer.clear();
    }
    _defaultLayer.clear();
    for (auto &layer : _frontLayers) {
        layer.clear();
    }
    _entitiesNb = 0;
}

static std::vector<std::size_t> match(std::vector<std::size_t> fst, std::vector<std::size_t> scd)
{
    std::vector<std::size_t> res;

    for (auto &it : fst) {
        for (auto &scdIt : scd) {
            if (it == scdIt) {
                res.push_back(it);
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

std::vector<std::size_t> Registry::getEntitiesByComponents(std::vector<std::type_index> types)
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
    std::cout << "setToBack " << id << std::endl;
    removeFromDefaultLayer(id);
    _backLayers[layer].push_back(id);
}

void Registry::setToDefaultLayer(std::size_t id)
{
    std::cout << "setToDefault " << id << std::endl;
    _defaultLayer.push_back(id);
}

void Registry::setToFrontLayers(std::size_t id, FrontLayers layer)
{
    std::cout << "setToFront " << id << std::endl;
    removeFromDefaultLayer(id);
    _frontLayers[layer].push_back(id);
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
        std::vector<std::vector<std::size_t>> &layers = back ? _backLayers : _frontLayers;
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

Clock &Registry::getClock()
{
    return _clock;
}

Logger::Logger &Registry::getLogger()
{
    return _logger;
}
