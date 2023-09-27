/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
Registry Registry::_instance = Registry();
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

Registry &Registry::getInstance()
{
    return _instance;
}

void Registry::addEntity()
{
    for (auto function : _addComponentPlaceFunctions) {
        function(*this);
    }
    _entitiesNb++;
}

void Registry::removeEntity(std::size_t id)
{
    for (auto function : _removeComponentFunctions) {
        function(*this, id);
    }
}

void Registry::clear()
{
    _data.clear();
    _addComponentPlaceFunctions.clear();
    _removeComponentFunctions.clear();
}

Registry::Registry() : _entitiesNb(0)
{
}
