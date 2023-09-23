/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Registry
*/

#include "Registry.hpp"

Registry Registry::_instance = Registry();

Registry &Registry::getInstance()
{
    return _instance;
}
