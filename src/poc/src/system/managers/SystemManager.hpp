/*
** SystemManager.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system/managers
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 10:25:23 AM 2023 brice
** Last update Thu Sep 20 11:23:54 AM 2023 brice
*/

#pragma once

#include <memory>
#include <vector>
#include "ISystem.hpp"

namespace System {
    class SystemManager {
        public:
            SystemManager();
            ~SystemManager();

            void updateSystems();

            void addSystem(std::unique_ptr<ISystem> system);
    
        protected:
            std::vector<std::unique_ptr<ISystem>> _systems;
    };
}
