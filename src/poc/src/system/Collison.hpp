/*
** Collison.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 9:36:35 AM 2023 brice
** Last update Thu Sep 20 9:58:17 AM 2023 brice
*/

#pragma once

#include "ASystem.hpp"

namespace System {
    class Collison : public ASystem {
        public:
            Collison(const Registry &registry);

            void run() final;
        protected:
        private:
    };
}
