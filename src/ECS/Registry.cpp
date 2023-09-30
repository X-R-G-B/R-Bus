/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"
#include <string>

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

void Registry::clear(std::vector<CustomIndex> indexes)
{
    _data.clear();
    _addComponentPlaceFunctions.clear();
    _removeComponentFunctions.clear();
    _entitiesNb = 0;
    _customSparseArrays.clear();
    initCustomSparseArrays(indexes);
}

std::size_t Registry::getEntitiesNb() const
{
    return (_entitiesNb);
}

void Registry::initCustomSparseArrays(std::vector<CustomIndex> indexes)
{
    for (auto index : indexes) {
        addCustomSparseIndex(static_cast<std::size_t>(index));
    }
}

Registry::Registry() : _entitiesNb(0), _maxCustomId(0)
{
}

void Registry::addCustomSparseIndex(std::size_t id)
{
    if (_customSparseArrays.find(id) != _customSparseArrays.end()) {
        throw std::runtime_error(
            "addCustomSparseIndex: id" + std::to_string(id) + " already exist");
    };
    _customSparseArrays[id] = SparseArray<std::size_t>();
    if (id > _maxCustomId) {
        _maxCustomId = id;
    }
}
