/*
** ASystem.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Tue Sep 19 5:18:35 PM 2023 brice
** Last update Thu Sep 20 9:57:34 AM 2023 brice
*/

#pragma once

#include "ISystem.hpp"
#include "Registry.hpp"

namespace System {
    class ASystem : public ISystem {
        public:
            ASystem(const Registry &registry);
            ~ASystem() = default;
            
        protected:
        private:
            void clientRun() final;

            Registry _registry;

    };
}
