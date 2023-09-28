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
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <vector>
#include "SparseArray.hpp"

enum CustomIndex { PLAYER, BULLET, ENNEMY, MAX };

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

        void clear();

        template <class Component>
        components<Component> getCustomSparseArray(std::size_t id)
        {
            if (id > _customSparseArrays.size()) {
                throw std::runtime_error("ID not in ");
            }
            try {
                components<Component> castedComponent =
                    std::any_cast<components<Component>>(
                        _customSparseArrays[id]);

                return castedComponent;
            } catch (const std::bad_any_cast &e) {
                throw std::runtime_error("Bad cast: " + std::string(e.what()));
            }
        }

        std::size_t getEntitiesNb() const;

        Registry &operator=(const Registry &) = delete;
        Registry(const Registry &)            = delete;
        void operator=(const Registry &&)     = delete;
        Registry(Registry &&)                 = delete;

    private:
        Registry();

        template <class Component>
        std::size_t addCustomSparseArray()
        {
            std::size_t id = _customSparseArrays.size();

            _customSparseArrays.push_back(SparseArray<Component>());
            return (id);
        }

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

        // NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
        static Registry _instance;
        // NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

        std::vector<std::function<void(Registry &)>>
            _addComponentPlaceFunctions;
        std::vector<std::function<void(Registry &, std::size_t)>>
            _removeComponentFunctions;
        std::unordered_map<std::type_index, std::any> _data;
        std::vector<std::any> _customSparseArrays;
        std::size_t _entitiesNb;
};
