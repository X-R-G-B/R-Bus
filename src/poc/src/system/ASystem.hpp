/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ASystem
*/

#pragma once

#include "ISystem.hpp"
#include "Registry.hpp"

namespace System {
    class ASystem : public ISystem {
        public:
            ASystem(Registry *registry);
            ~ASystem() = default;
            
        protected:
            Registry *_registry;
        private:
            void clientRun() final;


    };
}
