/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include <unordered_set>
#include "Registry.hpp"
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
#ifdef GRAPHICS
    unloadRaylibComponents(id);
#endif
    for (auto function : _removeComponentFunctions) {
        function(*this, id);
    }
    _entitiesNb--;
    for (auto &layer : _backLayers) {
        removeEntityFromLayer(id, layer);
    }
    removeEntityFromLayer(id, _defaultLayer);
    for (auto &layer : _frontLayers) {
        removeEntityFromLayer(id, layer);
    }
}

void Registry::clear()
{
    // Call unload functions for raylib components
#ifdef GRAPHICS
    for (std::size_t i = 0; i < _entitiesNb; i++) {
        unloadRaylibComponents(i);
    }
#endif

    _data.clear();
    _addComponentPlaceFunctions.clear();
    _removeComponentFunctions.clear();
    _getExistingsId.clear();
    clearAllies();
    _entitiesNb = 0;

    // Clear sprites layers
    for (auto &layer : _backLayers) {
        layer.clear();
    }
    _defaultLayer.clear();
    for (auto &layer : _frontLayers) {
        layer.clear();
    }
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

std::vector<std::size_t> Registry::getEntitiesWithOneOfComponents(std::vector<std::type_index> types)
{
    std::vector<std::size_t> vectorRes;
    std::unordered_set<std::size_t> res;

    for (auto type : types) {
        std::vector<std::size_t> tmp = getExistings(type);
        res.insert(tmp.begin(), tmp.end());
    }
    vectorRes.insert(vectorRes.end(), res.begin(), res.end());
    return vectorRes;
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

void Registry::addAllie(std::size_t typeId, std::type_index type)
{
    if (std::find(_allies[typeId].begin(), _allies[typeId].end(), type) == _allies[typeId].end()) {
        _allies[typeId].push_back(type);
    }
}

void Registry::clearAllies(std::size_t typeId)
{
    _allies[typeId].clear();
}

void Registry::clearAllies()
{
    _allies.clear();
}

bool Registry::checkAllies(std::size_t fstId, std::size_t scdId)
{
    Registry &registry = Registry::getInstance();

    for (auto group : _allies) {
        std::vector<std::size_t> ids = registry.getEntitiesWithOneOfComponents({group.second});
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
