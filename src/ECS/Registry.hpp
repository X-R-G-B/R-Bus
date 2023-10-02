/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#pragma once

#include <any>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "SceneManager.hpp"
#include "SparseArray.hpp"

enum LayerType { BACKLAYER, FRONTLAYER, DEFAULTLAYER };

enum BackLayers { BACK = 0, BACKMAX };

/*
 * FRONT is the frontest layer, so when adding a new one increment the FRONT
 * value and add the new one above
 */

enum FrontLayers { FRONT = 0, FRONTMAX };

class Registry {
    public:
        template <class Component>
        using components = SparseArray<Component> &;

        static Registry &getInstance();

        template <class Component>
        components<Component> getComponents()
        {
            checkAddSparseArray<Component>();
            return castReturn<Component>();
        }

        std::size_t addEntity();

        void removeEntity(std::size_t /*id*/);

        void clear();

        std::vector<std::size_t>
            getEntitiesByComponents(std::vector<std::type_index>);

        void
        setToBackLayers(std::size_t id, BackLayers layer = BackLayers::BACK);

        void setToDefaultLayer(std::size_t id);

        void setToFrontLayers(
            std::size_t id,
            FrontLayers layer = FrontLayers::FRONT);

        std::vector<std::vector<std::size_t>> getBackLayers();

        std::vector<std::size_t> getDefaultLayer();

        std::vector<std::vector<std::size_t>> getFrontLayers();

        Registry &operator=(const Registry &) = delete;
        Registry(const Registry &)            = delete;
        void operator=(const Registry &&)     = delete;
        Registry(Registry &&)                 = delete;

    private:
        Registry();

        void initLayers(bool back);

        void removeFromDefaultLayer(std::size_t id);

        std::vector<std::size_t> getExistings(std::type_index type);

        template <typename Component>
        void checkAddSparseArray()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = SparseArray<Component>();
                _addComponentPlaceFunctions.push_back(
                    &Registry::addComponentPlace<Component>);
                _removeComponentFunctions.push_back(
                    &Registry::removeComponent<Component>);
                _getExistingsId[typeid(Component)] =
                    &Registry::getExistingsId<Component>;
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
            return std::any_cast<components<Component>>(
                _data[typeid(Component)]);
        }

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Registry _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        std::vector<std::function<void(Registry &)>>
            _addComponentPlaceFunctions;
        std::vector<std::function<void(Registry &, std::size_t)>>
            _removeComponentFunctions;
        std::unordered_map<
            std::type_index,
            std::function<std::vector<std::size_t>(Registry &)>>
            _getExistingsId;
        std::unordered_map<std::type_index, std::any> _data;

        std::size_t _entitiesNb;

        std::vector<std::vector<std::size_t>> _backLayers;
        std::vector<std::size_t> _defaultLayer;
        std::vector<std::vector<std::size_t>> _frontLayers;
};
