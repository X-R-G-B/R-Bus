/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"

Registry Registry::_instance = Registry();

Registry &Registry::getInstance() {
    return _instance;
}

void Registry::addEntity()
{
    for (auto function : _addComponentPlaceFunctions) {
        function(*this);
    }
}

void Registry::removeEntity(std::size_t id)
{
    for (auto function : _removeComponentFunctions) {
        function(*this, id);
    }
}
