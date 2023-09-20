/*
** Movement.hpp for R-Bus in /home/kitetsu/Epitech/R-Bus/src/system
**
** Made by brice
** Login   <brice.desousa@epitech.eu>
**
** Started on  Wed Sep 20 10:19:50 AM 2023 brice
** Last update Thu Sep 20 10:28:46 AM 2023 brice
*/

#pragma once

#include "ASystem.hpp"

namespace System {
    class Movement : public ASystem {
        public:
            Movement(Registry *registry);

            void run() final;
        protected:
        private:
    };
}
