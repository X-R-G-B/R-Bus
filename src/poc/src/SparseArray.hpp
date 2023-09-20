/*
** SparseArray.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/poc/src
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 4:21:15 PM 2023 brice
** Last update Wed Sep 20 4:21:15 PM 2023 brice
*/

#include <iterator>
#include <list>

template <typename Component>
class SparseArray {
    public:
        void add(Component component)
        {
            _components.push_back(component);
        };
        void erase(int id)
        {
            auto it = _components.begin();
            std::advance(it, id);
            _components.erase(it);
        };
        Component &operator[](size_t idx)
        {
            return _components[idx];
        }

        std::list<Component>::iterator begin()
        {
            return _components.begin();
        }

        std::list<Component>::iterator end()
        {
            return _components.end();
        }
    private:
        std::list<Component> _components;
};
