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
        auto operator[](size_t idx) -> std::optional<Component> &
        {
            return _components[idx];
        }

        auto begin() -> std::list<std::optional<Component>>::iterator
        {
            return _components.begin();
        }

        auto end() -> std::list<std::optional<Component>>::iterator
        {
            return _components.end();
        }
        auto back() -> std::optional<Component> &
        {
            return _components.back();
        }

    private:
        std::list<std::optional<Component>> _components;
};
