/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SystemManager
*/

#pragma once

#include <functional>
#include <mutex>
#include <vector>

namespace Systems {
    class SystemManager {
        public:
            SystemManager(std::size_t id, std::vector<std::function<void(std::size_t, std::size_t)>> systems)
                : _id(id),
                  _originalSystems(std::move(systems)),
                  _modifiedSystems(_originalSystems),
                  _modified(false) {}
            void updateSystems()
            {
                std::size_t i        = 0;
                std::size_t decrease = 0;

                _toRemove.clear();
                auto &currentSystems = getSystems();
                for (auto &system : currentSystems) {
                    system(_id, i);
                    i++;
                }
                std::sort(_toRemove.begin(), _toRemove.end());
                for (auto &id : _toRemove) {
                    auto it = _modifiedSystems.begin();
                    std::advance(it, id - decrease);
                    _modifiedSystems.erase(it);
                    decrease++;
                }
            }

            void addSystem(std::function<void(std::size_t, std::size_t)> sys)
            {
                if (!_modified) {
                    _modified = true;
                }
                _modifiedSystems.push_back(sys);
            }

            void removeSystem(std::size_t id)
            {
                if (!_modified) {
                    _modified = true;
                }
                _toRemove.push_back(id);
            }

            void resetChanges()
            {
                _modified        = false;
                _modifiedSystems = _originalSystems;
            }

        private:
            std::vector<std::function<void(std::size_t, std::size_t)>> &getSystems()
            {
                if (_modified) {
                    return _modifiedSystems;
                }
                return _originalSystems;
            }

            std::size_t _id;
            std::vector<std::size_t> _toRemove;
            std::vector<std::function<void(std::size_t, std::size_t)>> _originalSystems;
            std::vector<std::function<void(std::size_t, std::size_t)>> _modifiedSystems;
            bool _modified;
    };
} // namespace Systems
