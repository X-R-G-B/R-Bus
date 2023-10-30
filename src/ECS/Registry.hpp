/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#pragma once

#include <any>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include <unordered_set>
#include "Clock.hpp"
#include "SparseArray.hpp"
#include "Logger.hpp"

enum LayerType { BACKLAYER, FRONTLAYER, DEFAULTLAYER };

enum BackLayers { BACK = 0, BACKMAX };

/*
 * FRONT is the frontest layer, so when adding a new one increment the FRONT
 * value and add the new one above
*/

enum FrontLayers { FRONT = 0, FRONTMAX };

enum Events {REMOVE_ENTITY, TAKE_DAMAGE, CLEAR, ENTITY_DEATH, START_LOOP, END_LOOP, BEFORE_LOOP, AFTER_LOOP};

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

class Registry {
    public:
        template <class Component>
        using components = SparseArray<Component> &;

        static Registry &getInstance()
        {
            static Registry instance;
            return instance;
        }

        template <class Component>
        components<Component> getComponents()
        {
            checkAddSparseArray<Component>();
            return castReturn<Component>();
        }

        std::size_t addEntity()
        {
            for (auto function : _addComponentPlaceFunctions) {
                function(*this);
            }
            _entitiesNb++;
            return _entitiesNb - 1;
        }

        void removeEntity(std::size_t id)
        {
            callback(Events::REMOVE_ENTITY, id);

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

        void removeEntitiesToRemove()
        {
            std::sort(_toRemove.begin(), _toRemove.end());
            std::size_t decrease = 0;
            for (auto id : _toRemove) {
                removeEntity(id - decrease);
                decrease++;
            }
            _toRemove.clear();
        }

        void addToRemove(std::size_t id)
        {
            if (std::find(_toRemove.begin(), _toRemove.end(), id) != _toRemove.end()) {
                return;
            }
            _toRemove.push_back(id);
        }

        void clear()
        {
            for (std::size_t i = 0; i < _entitiesNb; i++) {
                callback(Events::CLEAR, i);
            }

            _data.clear();
            _addComponentPlaceFunctions.clear();
            _removeComponentFunctions.clear();
            _getExistingsId.clear();
            clearAllies();
            _entitiesNb = 0;
            _toRemove.clear();

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

        std::vector<std::size_t> getEntitiesByComponents(std::vector<std::type_index> types)
        {
            auto it                      = types.begin();
            std::vector<std::size_t> res = getExistings(*it);
            it++;

            for (; it != types.end(); it++) {
                res = match(res, getExistings(*it));
            }
            return res;
        }

        std::vector<std::size_t> getEntitiesWithOneOfComponents(std::vector<std::type_index> types)
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

        void setToBackLayers(std::size_t id, BackLayers layer = BackLayers::BACK)
        {
            removeFromDefaultLayer(id);
            _backLayers[layer].push_back(id);
        }

        void setToDefaultLayer(std::size_t id)
        {
            _defaultLayer.push_back(id);
        }

        void setToFrontLayers(std::size_t id, FrontLayers layer = FrontLayers::FRONT)
        {
            removeFromDefaultLayer(id);
            _frontLayers[layer].push_back(id);
        }

        std::vector<std::vector<std::size_t>> getBackLayers()
        {
            return _backLayers;
        }

        std::vector<std::size_t> getDefaultLayer()
        {
            return _defaultLayer;
        }

        std::vector<std::vector<std::size_t>> getFrontLayers()
        {
            return _frontLayers;
        }

        void addAllie(std::size_t typeId, std::type_index type)
        {
            if (std::find(_allies[typeId].begin(), _allies[typeId].end(), type) == _allies[typeId].end()) {
                _allies[typeId].push_back(type);
            }
        }

        void clearAllies(std::size_t typeId)
        {
            _allies[typeId].clear();
        }

        void clearAllies()
        {
            _allies.clear();
        }

        bool checkAllies(std::size_t fstId, std::size_t scdId)
        {
            Registry &registry = Registry::getInstance();

            for (auto group : _allies) {
                std::vector<std::size_t> ids = registry.getEntitiesWithOneOfComponents({group.second});
                if (std::find(ids.begin(), ids.end(), fstId) != ids.end() && std::find(ids.begin(), ids.end(), scdId) != ids.end()) {
                    return true;
                }
            }
            return false;
        }

        void addEventCallback(Events event, std::function<void(std::size_t)> callback)
        {
            _eventsCallbacks[event].push_back(callback);
        }

        void callback(Events event, std::size_t id)
        {
            for (auto callback_ : _eventsCallbacks[event]) {
                callback_(id);
            }
        }

        void callback(Events event)
        {
            for (auto callback_ : _eventsCallbacks[event]) {
                callback_(0);
            }
        }

        Clock &getClock()
        {
            return _clock;
        }

        std::mutex mutex;

    private:
        Registry() : _entitiesNb(0)
        {
            initLayers(true);
        }

        Clock _clock;

        void initLayers(bool back)
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

        void removeFromDefaultLayer(std::size_t id)
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

        std::vector<std::size_t> getExistings(std::type_index type)
        {
            auto funcIt = _getExistingsId.find(type);
            if (funcIt == _getExistingsId.end()) {
                return {};
            }
            return (funcIt->second)(*this);
        }

        template <typename Component>
        void checkAddSparseArray()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = SparseArray<Component>();
                _addComponentPlaceFunctions.push_back(&Registry::addComponentPlace<Component>);
                _removeComponentFunctions.push_back(&Registry::removeComponent<Component>);
                _getExistingsId[typeid(Component)]   = &Registry::getExistingsId<Component>;
                components<Component> componentArray = castReturn<Component>();
                for (std::size_t i = 0; i < _entitiesNb; i++) {
                    componentArray.add();
                }
            }
        }

        template <typename Component>
        void addComponentPlace()
        {
            castReturn<Component>().add();
        }

        template <typename Component>
        void removeComponent(std::size_t id)
        {
            castReturn<Component>().erase(id);
        }

        template <typename Component>
        std::vector<std::size_t> getExistingsId()
        {
            return castReturn<Component>().getExistingsId();
        }

        template <class Component>
        components<Component> castReturn()
        {
            return std::any_cast<components<Component>>(_data[typeid(Component)]);
        }

        std::vector<std::function<void(Registry &)>> _addComponentPlaceFunctions;
        std::vector<std::function<void(Registry &, std::size_t)>> _removeComponentFunctions;
        std::unordered_map<std::type_index, std::function<std::vector<std::size_t>(Registry &)>>
            _getExistingsId;
        std::unordered_map<std::type_index, std::any> _data;

        std::size_t _entitiesNb;

        std::vector<std::vector<std::size_t>> _backLayers;
        std::vector<std::size_t> _defaultLayer;
        std::vector<std::vector<std::size_t>> _frontLayers;

        std::unordered_map<std::size_t, std::vector<std::type_index>> _allies;

        std::vector<std::size_t> _toRemove;

        std::unordered_map<Events, std::vector<std::function<void(std::size_t)>>> _eventsCallbacks;
};
