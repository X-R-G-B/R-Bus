/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include <any>
#include <typeinfo>
#include <typeindex>
#include <unordered_map>
#include "SparseArray.hpp"
#include <memory>

class Registry {
    public:
        template <class Component>
        using array = std::shared_ptr<SparseArray<Component>>;

        template <class Component>
        array<Component> registerComponent()
        {
            if (_data.find(typeid(Component)) == _data.end()) {
                _data[typeid(Component)] = std::make_shared<SparseArray<Component>>();
            }
            return castReturn<Component>();
        };
        template <class Component>
        array<Component> getComponents()
        {
            return castReturn<Component>();
        };
        template <class Component>
        array<Component> const &getComponents() const
        {
            return castReturn<Component>();
        };
    private:
        template<class Component>
        array<Component> castReturn()
        {
            return std::any_cast<array<Component>>(_data[typeid(Component)]);
        }
        std::unordered_map<std::type_index, std::any> _data;
};
