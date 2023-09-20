/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** Movement
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
