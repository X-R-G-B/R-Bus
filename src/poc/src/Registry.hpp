/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#pragma once

#include <any>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <iostream>
#include "SparseArray.hpp"

class Registry {
    
    public:
        template <class Component>
        using array = SparseArray<Component> &;

        static Registry &getInstance();

        template <class Component>
        array<Component> registerComponent()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = SparseArray<Component>();
            }
            return castReturn<Component>();
        }

        template <class Component>
        array<Component> getComponents()
        {
            return castReturn<Component>();
        }

        template <class Component>
        array<Component> const &getComponents() const
        {
            return castReturn<Component>();
        }
    private:
        Registry() = default;

        template<class Component>
        array<Component> castReturn()
        {
            return std::any_cast<array<Component>>(_data[typeid(Component)]);
        }

        static Registry _instance;
        Registry& operator=(const Registry&) = delete;
        std::unordered_map<std::type_index, std::any> _data;
};
