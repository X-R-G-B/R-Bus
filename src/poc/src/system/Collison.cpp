/*
** Collison.cpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 9:36:31 AM 2023 brice
** Last update Thu Sep 20 12:03:08 PM 2023 brice
*/

#include <iostream>
#include "Collison.hpp"

namespace System {
    
    Collison::Collison(const Registry &registry)
        : ASystem(registry)
    {

    }

    void Collison::run()
    {
        std::cout << "Update Collision" << std::endl;
        // get Position sparseArray with Position component type index
        //h
        // get Dammage sparseArray with Dammage component type index
        // get Health sparse Array with Health component type index

        // if (sparseArray.size() == 0) {
        //      return ()
        // }
        // for (std::size_t i = 0, auto it = sparseArray.begin(); it != sparseArray.end(); it++) {
        //      Position  toCheck =  sparseArray[i];
        //      auto tmp = it;
        //      for (; tmp != sparseArray.end(); tmp++) {
        //          if ()
        //      }
        //}
    }
}
