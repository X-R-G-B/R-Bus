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
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "raylib.h"
#include "SceneManager.hpp"
#include "SparseArray.hpp"

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

        void addEntity();

        void removeEntity(std::size_t /*id*/);

        void clear(std::vector<CustomIndex>);

        template <class Component>
        components<Component> getCustomSparseArray(std::size_t id)
        {
            if (_customSparseArrays.find(id) == _customSparseArrays.end()) {
                throw std::runtime_error(
                    "getCustomSparseArray ID not in :" + std::to_string(id));
            }
            try {
                components<Component> castedComponent =
                    std::any_cast<components<Component>>(
                        _customSparseArrays[id]);

                return castedComponent;
            } catch (const std::bad_any_cast e) {
                throw std::runtime_error(
                    "getCustomSparseArray: Bad any cast. ID: "
                    + std::to_string(id));
            }
        }

        template <class Component>
        std::size_t addCustomSparseArray()
        {
            _maxCustomId++;
            std::size_t id = _maxCustomId;

            _customSparseArrays[id] = SparseArray<Component>();
            return (id);
        }

        std::size_t getEntitiesNb() const;

        void initCustomSparseArrays(std::vector<CustomIndex> indexes);

        Registry &operator=(const Registry &) = delete;
        Registry(const Registry &)            = delete;
        void operator=(const Registry &&)     = delete;
        Registry(Registry &&)                 = delete;

    private:
        Registry();

        template <typename Component>
        void checkAddSparseArray()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = SparseArray<Component>();
                _addComponentPlaceFunctions.push_back(
                    &Registry::addComponentPlace<Component>);
                _removeComponentFunctions.push_back(
                    &Registry::removeComponent<Component>);
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

        template <class Component>
        components<Component> castReturn()
        {
            return std::any_cast<components<Component>>(
                _data[typeid(Component)]);
        }

        void addCustomSparseIndex(std::size_t id);

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Registry _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        std::vector<std::function<void(Registry &)>>
            _addComponentPlaceFunctions;
        std::vector<std::function<void(Registry &, std::size_t)>>
            _removeComponentFunctions;
        std::unordered_map<std::type_index, std::any> _data;

        std::unordered_map<std::size_t, std::any> _customSparseArrays;
        std::size_t _maxCustomId;

        std::size_t _entitiesNb;
};
