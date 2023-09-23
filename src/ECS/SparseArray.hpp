/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SparseArray
*/

#pragma once

#include <iterator>
#include <list>
#include <optional>

template <typename Component> class SparseArray {
    public:
        void add()
        {
            _components.push_back(std::nullopt);
        }
        void erase(std::size_t id)
        {
            auto it = _components.begin();
            std::advance(it, id);
            _components.erase(it);
        };
        Component &operator[](size_t idx)
        {
            return _components[idx];
        }

        std::list<std::optional<Component>>::iterator begin()
        {
            return _components.begin();
        }

        std::list<std::optional<Component>>::iterator end()
        {
            return _components.end();
        }
        std::optional<Component> &back()
        {
            return _components.back();
        }

    private:
        std::list<std::optional<Component>> _components;
};
