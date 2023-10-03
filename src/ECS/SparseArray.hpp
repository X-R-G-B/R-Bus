/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** SparseArray
*/

#pragma once

#include <iterator>
#include <vector>

template <typename Component>
class SparseArray {
    public:
        void add()
        {
            _sparse.push_back(std::size_t(-1));
        }

        void insertBack(const Component &value)
        {
            insert(_sparse.size() - 1, value);
        }

        void insert(size_t id, const Component &value)
        {
            if (id >= _sparse.size()) {
                throw std::runtime_error(
                    "SparseArrays::insert: ID out of bounds!");
            }

            if (_sparse[id] > -1) {
                _dense[_sparse[id]]     = value;
                _revSparse[_sparse[id]] = id;
                return;
            }

            _sparse[id] = _dense.size();
            _dense.push_back(value);
            _revSparse.push_back(id);
        }

        void erase(std::size_t id)
        {
            if (id >= _sparse.size()) {
                throw std::runtime_error(
                    "SparseArrays::erase: ID out of bounds!");
            }
            std::size_t sparseValue = _sparse[id];
            if (sparseValue != -1) {
                removeDenses(id, sparseValue);
            }
            auto it = _sparse.begin();
            std::advance(it, id);
            _sparse.erase(it);
        };

        Component &operator[](size_t id)
        {
            throwIfDontExist(id);
            return _dense[_sparse[id]];
        }

        /*
         * A dense sparseArrays is not sort by entities id, the begin of two
         * sparseArrays could be different entities, only _sparse are
         * synchronized You can only use iterator in a system dealing with only
         * one component at time
         */
        typename std::vector<Component>::iterator begin()
        {
            return _dense.begin();
        }

        typename std::vector<Component>::iterator end()
        {
            return _dense.end();
        }

        bool exist(std::size_t id)
        {
            if (id >= _sparse.size()) {
                return false;
            }
            for (auto elemId : _revSparse) {
                if (id == elemId) {
                    return true;
                }
            }
            return false;
        }

        std::vector<std::size_t> getExistingsId()
        {
            return _revSparse;
        }

    private:
        void removeDenses(std::size_t id, std::size_t sparseValue)
        {
            auto it = _dense.begin();
            std::advance(it, sparseValue);
            _dense.erase(it);
            auto revIt = _revSparse.begin();
            std::advance(revIt, sparseValue);
            _revSparse.erase(revIt);
            for (auto revIt2 = _revSparse.begin(); revIt2 != _revSparse.end();
                 revIt2++) {
                if (*revIt2 > id) {
                    (*revIt2)--;
                }
            }
            for (auto it2 = _sparse.begin(); it2 != _sparse.end(); it2++) {
                if (*it2 > sparseValue) {
                    (*it2)--;
                }
            }
        }

        void throwIfDontExist(std::size_t id)
        {
            if (!exist(id)) {
                throw std::runtime_error("SparseArrays: ID out of bounds!");
            }
        }

        std::vector<Component> _dense;
        std::vector<std::size_t> _sparse;
        std::vector<std::size_t> _revSparse;
};
