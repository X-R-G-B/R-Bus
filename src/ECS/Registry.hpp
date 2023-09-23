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
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include "raylib.h"
#include "SparseArray.hpp"

class Registry {
    public:
        template <class Component> using components = SparseArray<Component> &;

        static auto getInstance() -> Registry &;

        template <class Component> auto getComponents() -> components<Component>
        {
            checkAddSparseArray<Component>();
            return castReturn<Component>();
        }

        void addEntity();

        void removeEntity(std::size_t /*id*/);

    private:
        Registry() = default;

        template <typename Component> void checkAddSparseArray()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = SparseArray<Component>();
                _addComponentPlaceFunctions.push_back(
                &Registry::addComponentPlace<Component>);
                _removeComponentFunctions.push_back(
                &Registry::removeComponent<Component>);
            }
        }

        template <typename Component> void addComponentPlace()
        {
            castReturn<Component>().add();
        }

        template <typename Component> void removeComponent(std::size_t id)
        {
            castReturn<Component>().erase(id);
        }

        template <class Component> auto castReturn() -> components<Component>
        {
            return std::any_cast<components<Component>>(
            _data[typeid(Component)]);
        }

        static Registry _instance;
        auto operator=(const Registry &) -> Registry & = delete;

        std::list<std::function<void(Registry &)>> _addComponentPlaceFunctions;
        std::list<std::function<void(Registry &, std::size_t)>>
        _removeComponentFunctions;
        std::unordered_map<std::type_index, std::any> _data;
};