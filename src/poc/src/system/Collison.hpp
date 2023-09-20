/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Collison
*/

#pragma once

#include "ASystem.hpp"

namespace System {
    class Collison : public ASystem {
        public:
            Collison(Registry *registry);

            void run() final;
        protected:
        private:
    };
}
