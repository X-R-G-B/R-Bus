/*
** EPITECH PROJECT, 2023
** R-Bus
** File description:
** ISystem
*/

#pragma once

namespace System {
    class ISystem {
        public:
            virtual ~ISystem() = default;

            virtual void run() = 0;

        protected:
        private:
            virtual void clientRun() = 0;
    };
}
